#import "PeripheralBaseMacOS.h"
#import "CharacteristicBuilder.h"
#import "DescriptorBuilder.h"
#import "LoggingInternal.h"
#import "ServiceBuilder.h"
#import "Utils.h"

#import <simpleble/Exceptions.h>

#import <iostream>

#define WAIT_UNTIL_FALSE(obj, var)                \
    do {                                          \
        BOOL _tmpVar = YES;                       \
        while (_tmpVar) {                         \
            [NSThread sleepForTimeInterval:0.01]; \
            @synchronized(obj) {                  \
                _tmpVar = (obj->var);             \
            }                                     \
        }                                         \
    } while (0)

struct ble_task_t {
    BOOL pending;
    NSError* error;

    ble_task_t() : pending(false), error(nil) {}
};

struct descriptor_extras_t {
    ble_task_t readTask;
    ble_task_t writeTask;

    descriptor_extras_t() {}
};

struct characteristic_extras_t {
    ble_task_t readTask;
    ble_task_t writeTask;
    ble_task_t notifyTask;
    std::map<std::string, descriptor_extras_t> descriptor_extras;
    std::function<void(SimpleBLE::ByteArray)> valueChangedCallback;

    characteristic_extras_t() {}
};

@interface PeripheralBaseMacOS () {
    BOOL connectionPending_;
    BOOL disconnectionPending_;
    BOOL serviceDiscoveryPending_;
    BOOL characteristicDiscoveryPending_;
    BOOL descriptorDiscoveryPending_;

    std::mutex ble_task_lock_;

    // NOTE: This dictionary assumes that all characteristic UUIDs are unique, which could not always be the case.
    std::map<std::string, characteristic_extras_t> characteristic_extras_;
}

// Private properties
@property(strong) NSError* lastError_;
@property(strong) CBPeripheral* peripheral;
@property(strong) CBCentralManager* centralManager;

- (CBService*)findService:(NSString*)uuid;
- (CBCharacteristic*)findCharacteristic:(NSString*)uuid service:(CBService*)service;
- (std::pair<CBService*, CBCharacteristic*>)findServiceAndCharacteristic:(NSString*)service_uuid
                                                     characteristic_uuid:(NSString*)characteristic_uuid;
- (void)throwBasedOnError:(NSError*)error withFormat:(NSString*)format, ...;

@end

@implementation PeripheralBaseMacOS

- (instancetype)init:(CBPeripheral*)peripheral centralManager:(CBCentralManager*)centralManager {
    self = [super init];
    if (self) {
        // NOTE: It's important to make a copy of the peripheral and central objects into
        // a strong property to prevent them from being deallocated by ARC or the garbage collector.
        _peripheral = [peripheral copy];
        _centralManager = centralManager;

        _peripheral.delegate = self;
    }
    return self;
}

- (void*)underlying {
    return (__bridge void*)self.peripheral;
}

- (NSString*)identifier {
    if (self.peripheral.name != nil) {
        return [self.peripheral.name copy];
    } else {
        return @"";
    }
}

- (NSString*)address {
    return [self.peripheral.identifier UUIDString];
}

- (uint16_t)mtu {
    return [self.peripheral maximumWriteValueLengthForType:CBCharacteristicWriteWithoutResponse];
}

- (void)connect {
    std::lock_guard<std::mutex> lg(ble_task_lock_);

    // --- Connect to the peripheral ---
    @synchronized(self) {
        self.lastError_ = nil;
        self->connectionPending_ = YES;
        [self.centralManager connectPeripheral:self.peripheral options:@{}];  // TODO: Do we need to pass any options?
    }

    WAIT_UNTIL_FALSE(self, connectionPending_);

    if (self.peripheral.state != CBPeripheralStateConnected) {
        [self throwBasedOnError:self.lastError_ withFormat:@"Peripheral Connection"];
    }

    // --- Discover services and characteristics ---

    @synchronized(self) {
        self.lastError_ = nil;
        self->serviceDiscoveryPending_ = YES;
        [self.peripheral discoverServices:nil];
    }

    WAIT_UNTIL_FALSE(self, serviceDiscoveryPending_);

    if (self.peripheral.services == nil || self.peripheral.state != CBPeripheralStateConnected) {
        [self throwBasedOnError:self.lastError_ withFormat:@"Service Discovery"];
    }

    // For each service found, discover characteristics.
    for (CBService* service in self.peripheral.services) {
        @synchronized(self) {
            self.lastError_ = nil;
            self->characteristicDiscoveryPending_ = YES;
            [self.peripheral discoverCharacteristics:nil forService:service];
        }

        WAIT_UNTIL_FALSE(self, characteristicDiscoveryPending_);

        if (service.characteristics == nil || self.peripheral.state != CBPeripheralStateConnected) {
            [self throwBasedOnError:self.lastError_ withFormat:@"Characteristic Discovery for service %@", service.UUID];
        }

        // For each characteristic, create the associated extra properties and discover descriptors.
        for (CBCharacteristic* characteristic in service.characteristics) {
            @synchronized(self) {
                self.lastError_ = nil;
                self->descriptorDiscoveryPending_ = YES;
                [self.peripheral discoverDescriptorsForCharacteristic:characteristic];
            }

            WAIT_UNTIL_FALSE(self, descriptorDiscoveryPending_);

            if (characteristic.descriptors == nil || self.peripheral.state != CBPeripheralStateConnected) {
                [self throwBasedOnError:self.lastError_ withFormat:@"Descriptor Discovery for characteristic %@", characteristic.UUID];
            }

            @synchronized(self) {
                characteristic_extras_t characteristic_extra;
                for (CBDescriptor* descriptor in characteristic.descriptors) {
                    characteristic_extra.descriptor_extras[uuidToSimpleBLE(descriptor.UUID)] = {};
                }
                characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)] = characteristic_extra;
            }
        }
    }
}

- (void)disconnect {
    std::lock_guard<std::mutex> lg(ble_task_lock_);

    @synchronized(self) {
        self.lastError_ = nil;
        self->disconnectionPending_ = YES;
        [self.centralManager cancelPeripheralConnection:self.peripheral];
    }

    WAIT_UNTIL_FALSE(self, disconnectionPending_);

    if (self.peripheral.state != CBPeripheralStateDisconnected) {
        [self throwBasedOnError:self.lastError_ withFormat:@"Peripheral Disconnection"];
    }
}

- (bool)isConnected {
    return self.peripheral.state == CBPeripheralStateConnected;
}

- (std::vector<SimpleBLE::Service>)getServices {
    std::vector<SimpleBLE::Service> service_list;
    for (CBService* service in self.peripheral.services) {
        // Build the list of characteristics for the service.
        std::vector<SimpleBLE::Characteristic> characteristic_list;
        for (CBCharacteristic* characteristic in service.characteristics) {
            // Build the list of descriptors for the characteristic.
            std::vector<SimpleBLE::Descriptor> descriptor_list;
            for (CBDescriptor* descriptor in characteristic.descriptors) {
                descriptor_list.push_back(SimpleBLE::DescriptorBuilder(uuidToSimpleBLE(descriptor.UUID)));
            }

            bool can_read = (characteristic.properties & CBCharacteristicPropertyRead) != 0;
            bool can_write_request = (characteristic.properties & CBCharacteristicPropertyWrite) != 0;
            bool can_write_command = (characteristic.properties & CBCharacteristicPropertyWriteWithoutResponse) != 0;
            bool can_notify = (characteristic.properties & CBCharacteristicPropertyNotify) != 0;
            bool can_indicate = (characteristic.properties & CBCharacteristicPropertyIndicate) != 0;

            characteristic_list.push_back(SimpleBLE::CharacteristicBuilder(uuidToSimpleBLE(characteristic.UUID), descriptor_list, can_read,
                                                                           can_write_request, can_write_command, can_notify, can_indicate));
        }
        service_list.push_back(SimpleBLE::ServiceBuilder(uuidToSimpleBLE(service.UUID), characteristic_list));
    }

    return service_list;
}

- (SimpleBLE::ByteArray)read:(NSString*)service_uuid characteristic_uuid:(NSString*)characteristic_uuid {
    std::lock_guard<std::mutex> lg(ble_task_lock_);

    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    // Check that the characteristic supports this feature.
    if ((characteristic.properties & CBCharacteristicPropertyRead) == 0) {
        NSLog(@"Characteristic does not support read.");
        throw SimpleBLE::Exception::OperationNotSupported();
    }

    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].readTask;

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        [self.peripheral readValueForCharacteristic:characteristic];
    }

    WAIT_UNTIL_FALSE(self, characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].readTask.pending);

    if (task.error != nil) {
        [self throwBasedOnError:task.error withFormat:@"Characteristic %@ Read", characteristic.UUID];
    }

    return SimpleBLE::ByteArray((const char*)characteristic.value.bytes, characteristic.value.length);
}

- (void)writeRequest:(NSString*)service_uuid characteristic_uuid:(NSString*)characteristic_uuid payload:(NSData*)payload {
    std::lock_guard<std::mutex> lg(ble_task_lock_);

    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    // Check that the characteristic supports this feature.
    if ((characteristic.properties & CBCharacteristicPropertyWrite) == 0) {
        NSLog(@"Characteristic does not support write with response.");
        throw SimpleBLE::Exception::OperationNotSupported();
    }

    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].writeTask;

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        [self.peripheral writeValue:payload forCharacteristic:characteristic type:CBCharacteristicWriteWithResponse];
    }

    WAIT_UNTIL_FALSE(self, characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].writeTask.pending);

    if (task.error != nil) {
        [self throwBasedOnError:task.error withFormat:@"Characteristic %@ Write Request", characteristic.UUID];
    }
}

- (void)writeCommand:(NSString*)service_uuid characteristic_uuid:(NSString*)characteristic_uuid payload:(NSData*)payload {
    std::lock_guard<std::mutex> lg(ble_task_lock_);

    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    // Check that the characteristic supports this feature.
    if ((characteristic.properties & CBCharacteristicPropertyWriteWithoutResponse) == 0) {
        NSLog(@"Characteristic does not support write without response.");
        throw SimpleBLE::Exception::OperationNotSupported();
    }

    // NOTE: This write is unacknowledged.
    @synchronized(self) {
        [self.peripheral writeValue:payload forCharacteristic:characteristic type:CBCharacteristicWriteWithoutResponse];
    }
}

- (void)notify:(NSString*)service_uuid
    characteristic_uuid:(NSString*)characteristic_uuid
               callback:(std::function<void(SimpleBLE::ByteArray)>)callback {
    std::lock_guard<std::mutex> lg(ble_task_lock_);

    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].notifyTask;

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].valueChangedCallback = callback;
        [self.peripheral setNotifyValue:YES forCharacteristic:characteristic];
    }

    WAIT_UNTIL_FALSE(self, characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].notifyTask.pending);

    if (!characteristic.isNotifying || task.error != nil) {
        [self throwBasedOnError:task.error withFormat:@"Characteristic %@ Notify/Indicate", characteristic.UUID];
    }
}

- (void)indicate:(NSString*)service_uuid
    characteristic_uuid:(NSString*)characteristic_uuid
               callback:(std::function<void(SimpleBLE::ByteArray)>)callback {
    [self notify:service_uuid characteristic_uuid:characteristic_uuid callback:callback];
}

- (void)unsubscribe:(NSString*)service_uuid characteristic_uuid:(NSString*)characteristic_uuid {
    std::lock_guard<std::mutex> lg(ble_task_lock_);

    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].notifyTask;

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].valueChangedCallback = nil;
        [self.peripheral setNotifyValue:NO forCharacteristic:characteristic];
    }

    WAIT_UNTIL_FALSE(self, characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].notifyTask.pending);

    if (characteristic.isNotifying || task.error != nil) {
        [self throwBasedOnError:task.error withFormat:@"Characteristic %@ Unsubscribe", characteristic.UUID];
    }
}

- (SimpleBLE::ByteArray)read:(NSString*)service_uuid
         characteristic_uuid:(NSString*)characteristic_uuid
             descriptor_uuid:(NSString*)descriptor_uuid {
    std::lock_guard<std::mutex> lg(ble_task_lock_);
    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;
    CBDescriptor* descriptor = [self findDescriptor:descriptor_uuid characteristic:characteristic];
    ble_task_t& task =
        characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].descriptor_extras[uuidToSimpleBLE(descriptor.UUID)].readTask;

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        [self.peripheral readValueForDescriptor:descriptor];
    }

    WAIT_UNTIL_FALSE(
        self,
        characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].descriptor_extras[uuidToSimpleBLE(descriptor.UUID)].readTask.pending);

    if (task.error != nil) {
        [self throwBasedOnError:task.error withFormat:@"Descriptor %@ Read", descriptor.UUID];
    }

    const char* bytes = (const char*)[descriptor.value bytes];

    return SimpleBLE::ByteArray(bytes, [descriptor.value length]);
}

- (void)write:(NSString*)service_uuid
    characteristic_uuid:(NSString*)characteristic_uuid
        descriptor_uuid:(NSString*)descriptor_uuid
                payload:(NSData*)payload {
    std::lock_guard<std::mutex> lg(ble_task_lock_);
    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;
    CBDescriptor* descriptor = [self findDescriptor:descriptor_uuid characteristic:characteristic];
    ble_task_t& task =
        characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].descriptor_extras[uuidToSimpleBLE(descriptor.UUID)].writeTask;

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        [self.peripheral writeValue:payload forDescriptor:descriptor];
    }

    WAIT_UNTIL_FALSE(
        self,
        characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].descriptor_extras[uuidToSimpleBLE(descriptor.UUID)].writeTask.pending);

    if (task.error) {
        [self throwBasedOnError:task.error withFormat:@"Descriptor %@ Write", descriptor.UUID];
    }
}

#pragma mark - Auxiliary methods

- (CBService*)findService:(NSString*)uuid {
    CBUUID* service_uuid = [CBUUID UUIDWithString:uuid];

    for (CBService* service in self.peripheral.services) {
        if ([service.UUID isEqual:service_uuid]) {
            return service;
        }
    }

    throw SimpleBLE::Exception::ServiceNotFound([uuid UTF8String]);
}

- (CBCharacteristic*)findCharacteristic:(NSString*)uuid service:(CBService*)service {
    CBUUID* characteristic_uuid = [CBUUID UUIDWithString:uuid];

    if (service == nil) {
        throw SimpleBLE::Exception::BaseException("Invalid service parameter.");
    }

    for (CBCharacteristic* characteristic in service.characteristics) {
        if ([characteristic.UUID isEqual:characteristic_uuid]) {
            return characteristic;
        }
    }

    throw SimpleBLE::Exception::CharacteristicNotFound([uuid UTF8String]);
}

- (CBDescriptor*)findDescriptor:(NSString*)uuid characteristic:(CBCharacteristic*)characteristic {
    CBUUID* descriptor_uuid = [CBUUID UUIDWithString:uuid];

    for (CBDescriptor* descriptor in characteristic.descriptors) {
        if ([descriptor.UUID isEqual:descriptor_uuid]) {
            return descriptor;
        }
    }

    throw SimpleBLE::Exception::DescriptorNotFound([uuid UTF8String]);
}

- (std::pair<CBService*, CBCharacteristic*>)findServiceAndCharacteristic:(NSString*)service_uuid
                                                     characteristic_uuid:(NSString*)characteristic_uuid {
    CBService* service = [self findService:service_uuid];
    CBCharacteristic* characteristic = [self findCharacteristic:characteristic_uuid service:service];
    return std::pair<CBService*, CBCharacteristic*>(service, characteristic);
}

- (void)throwBasedOnError:(NSError*)error withFormat:(NSString*)format, ... {
    va_list argList;
    va_start(argList, format);
    NSString* formattedString = [[NSString alloc] initWithFormat:format arguments:argList];
    va_end(argList);

    if (error == nil) {
        NSString* exceptionMessage = [NSString stringWithFormat:@"%@ failed", formattedString];
        NSLog(@"%@", exceptionMessage);
        throw SimpleBLE::Exception::OperationFailed([exceptionMessage UTF8String]);
    } else {
        NSString* errorMessage = [error localizedDescription];
        NSString* exceptionMessage = [NSString stringWithFormat:@"%@ failed: %@", formattedString, errorMessage];
        NSLog(@"%@", exceptionMessage);
        throw SimpleBLE::Exception::OperationFailed([exceptionMessage UTF8String]);
    }
}

#pragma mark - CBCentralManagerDelegate

- (void)delegateDidConnect {
    @synchronized(self) {
        self->connectionPending_ = NO;
    }
}

- (void)delegateDidFailToConnect:(NSError*)error {
    if (error != nil) {
        @synchronized(self) {
            self.lastError_ = error;
        }
    }

    @synchronized(self) {
        self->connectionPending_ = NO;
    }
}

- (void)delegateDidDisconnect:(NSError*)error {
    if (error != nil) {
        @synchronized(self) {
            self.lastError_ = error;
        }
    }

    @synchronized(self) {
        self->serviceDiscoveryPending_ = NO;
        self->characteristicDiscoveryPending_ = NO;
        self->descriptorDiscoveryPending_ = NO;
        self->disconnectionPending_ = NO;

        for (auto& characteristic_entry : self->characteristic_extras_) {
            characteristic_extras_t& characteristic_extra = characteristic_entry.second;

            characteristic_extra.readTask.pending = NO;
            characteristic_extra.writeTask.pending = NO;
            characteristic_extra.notifyTask.pending = NO;

            for (auto& descriptor_entry : characteristic_extra.descriptor_extras) {
                descriptor_extras_t& descriptor_extra = descriptor_entry.second;
                descriptor_extra.readTask.pending = NO;
                descriptor_extra.writeTask.pending = NO;
            }
        }
    }
}

#pragma mark - CBPeripheralDelegate

- (void)peripheral:(CBPeripheral*)peripheral didModifyServices:(NSArray<CBService*>*)invalidatedServices {
    // NOTE: Whenever this method is called, any pending operations are cancelled. In addition to that,
    //       the provided list of services does NOT include any characteristics or descriptors, so need to
    //       clear pending flags for those as well.

    NSLog(@"\n\n\ndidModifyServices: %@\n\n\n", invalidatedServices);

    @synchronized(self) {
        for (auto& characteristic_entry : self->characteristic_extras_) {
            characteristic_extras_t& characteristic_extra = characteristic_entry.second;

            characteristic_extra.readTask.pending = NO;
            characteristic_extra.writeTask.pending = NO;
            characteristic_extra.notifyTask.pending = NO;

            for (auto& descriptor_entry : characteristic_extra.descriptor_extras) {
                descriptor_extras_t& descriptor_extra = descriptor_entry.second;
                descriptor_extra.readTask.pending = NO;
                descriptor_extra.writeTask.pending = NO;
            }
        }
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didDiscoverServices:(NSError*)error {
    // NOTE: As we are currently polling the result of the discovery, this callback is not needed,
    // but might be useful in the future.
    if (error != nil) {
        @synchronized(self) {
            self.lastError_ = error;
        }
    }

    @synchronized(self) {
        self->serviceDiscoveryPending_ = NO;
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didDiscoverCharacteristicsForService:(CBService*)service error:(NSError*)error {
    // NOTE: As we are currently polling the result of the discovery, this callback is not needed,
    // but might be useful in the future.
    if (error != nil) {
        @synchronized(self) {
            self.lastError_ = error;
        }
    }

    @synchronized(self) {
        self->characteristicDiscoveryPending_ = NO;
    }
}

- (void)peripheral:(CBPeripheral*)peripheral
    didDiscoverDescriptorsForCharacteristic:(CBCharacteristic*)characteristic
                                      error:(NSError*)error {
    if (error != nil) {
        @synchronized(self) {
            self.lastError_ = error;
        }
    }

    @synchronized(self) {
        self->descriptorDiscoveryPending_ = NO;
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didUpdateValueForCharacteristic:(CBCharacteristic*)characteristic error:(NSError*)error {
    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].writeTask;

    @synchronized(self) {
        @synchronized(self) {
            task.error = error;
            if (task.pending) {
                task.pending = NO;
            }
        }

        // Check if the characteristic has a callback and call it
        if (characteristic.isNotifying && characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].valueChangedCallback != nil) {
            SimpleBLE::ByteArray received_data((const char*)characteristic.value.bytes, characteristic.value.length);
            characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].valueChangedCallback(received_data);
        }
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didWriteValueForCharacteristic:(CBCharacteristic*)characteristic error:(NSError*)error {
    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].writeTask;

    @synchronized(self) {
        task.error = error;
        if (task.pending) {
            task.pending = NO;
        }
    }
}

- (void)peripheral:(CBPeripheral*)peripheral
    didUpdateNotificationStateForCharacteristic:(CBCharacteristic*)characteristic
                                          error:(NSError*)error {
    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].notifyTask;

    @synchronized(self) {
        task.error = error;
        if (task.pending) {
            task.pending = NO;
        }
    }
}

- (void)peripheralIsReadyToSendWriteWithoutResponse:(CBPeripheral*)peripheral {
    NSLog(@"Peripheral ready to send: %@", peripheral);
}

- (void)peripheral:(CBPeripheral*)peripheral didUpdateValueForDescriptor:(CBDescriptor*)descriptor error:(NSError*)error {
    std::string characteristic_uuid = uuidToSimpleBLE(descriptor.characteristic.UUID);
    std::string descriptor_uuid = uuidToSimpleBLE(descriptor.UUID);
    ble_task_t& task = characteristic_extras_[characteristic_uuid].descriptor_extras[descriptor_uuid].readTask;

    @synchronized(self) {
        task.error = error;
        if (task.pending) {
            task.pending = NO;
        }
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didWriteValueForDescriptor:(CBDescriptor*)descriptor error:(NSError*)error {
    std::string characteristic_uuid = uuidToSimpleBLE(descriptor.characteristic.UUID);
    std::string descriptor_uuid = uuidToSimpleBLE(descriptor.UUID);
    ble_task_t& task = characteristic_extras_[characteristic_uuid].descriptor_extras[descriptor_uuid].writeTask;

    @synchronized(self) {
        task.error = error;
        if (task.pending) {
            task.pending = NO;
        }
    }
}

@end
