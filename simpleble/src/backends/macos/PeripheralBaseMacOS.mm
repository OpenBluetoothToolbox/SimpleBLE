#import "PeripheralBaseMacOS.h"
#import "CharacteristicBuilder.h"
#import "DescriptorBuilder.h"
#import "LoggingInternal.h"
#import "ServiceBuilder.h"
#import "Utils.h"

#import <simpleble/Exceptions.h>
#import <optional>

#define WAIT_UNTIL_FALSE(obj, var)                \
    do {                                          \
        BOOL _tmpVar = YES;                       \
        while (_tmpVar) {                         \
            [NSThread sleepForTimeInterval:0.01]; \
            @synchronized(obj) {                  \
                _tmpVar = (var);                  \
            }                                     \
        }                                         \
    } while (0)

struct ble_task_t {
    std::mutex lock;
    BOOL pending;
    NSError* error;

    ble_task_t() : pending(false), error(nil) {}
};

struct descriptor_extras_t {
    ble_task_t task;

    descriptor_extras_t() {}
};

struct characteristic_extras_t {
    ble_task_t task;

    std::optional<SimpleBLE::ByteArray> value;
    std::map<std::string, descriptor_extras_t> descriptor_extras;
    std::function<void(SimpleBLE::ByteArray)> valueChangedCallback;

    characteristic_extras_t() {}
};

@interface PeripheralBaseMacOS () {
    ble_task_t task_;
    NSError* disconnectionError_;

    // NOTE: This dictionary assumes that all characteristic UUIDs are unique, which could not always be the case.
    std::map<std::string, characteristic_extras_t> characteristic_extras_;
}

// Private properties
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
    std::lock_guard<std::mutex> lg(task_.lock);

    // --- Connect to the peripheral ---
    @synchronized(self) {
        task_.error = nil;
        task_.pending = YES;
        [self.centralManager connectPeripheral:self.peripheral options:@{}];  // TODO: Do we need to pass any options?
    }

    WAIT_UNTIL_FALSE(self, task_.pending);

    if (self.peripheral.state != CBPeripheralStateConnected) {
        [self throwBasedOnError:task_.error withFormat:@"Peripheral Connection"];
    }

    // --- Discover services and characteristics ---

    @synchronized(self) {
        task_.error = nil;
        task_.pending = YES;
        [self.peripheral discoverServices:nil];
    }

    WAIT_UNTIL_FALSE(self, task_.pending);

    if (self.peripheral.services == nil || self.peripheral.state != CBPeripheralStateConnected) {
        [self throwBasedOnError:task_.error withFormat:@"Service Discovery"];
    }

    // For each service found, discover characteristics.
    for (CBService* service in self.peripheral.services) {
        @synchronized(self) {
            task_.error = nil;
            task_.pending = YES;
            [self.peripheral discoverCharacteristics:nil forService:service];
        }

        WAIT_UNTIL_FALSE(self, task_.pending);

        if (service.characteristics == nil || self.peripheral.state != CBPeripheralStateConnected) {
            [self throwBasedOnError:task_.error withFormat:@"Characteristic Discovery for service %@", service.UUID];
        }

        // For each characteristic, create the associated extra properties and discover descriptors.
        for (CBCharacteristic* characteristic in service.characteristics) {
            @synchronized(self) {
                task_.error = nil;
                task_.pending = YES;
                [self.peripheral discoverDescriptorsForCharacteristic:characteristic];
            }

            WAIT_UNTIL_FALSE(self, task_.pending);

            if (characteristic.descriptors == nil || self.peripheral.state != CBPeripheralStateConnected) {
                [self throwBasedOnError:task_.error withFormat:@"Descriptor Discovery for characteristic %@", characteristic.UUID];
            }
        }
    }
}

- (void)disconnect {
    std::lock_guard<std::mutex> lg(task_.lock);

    @synchronized(self) {
        task_.error = nil;
        task_.pending = YES;
        [self.centralManager cancelPeripheralConnection:self.peripheral];
    }

    WAIT_UNTIL_FALSE(self, task_.pending);

    if (self.peripheral.state != CBPeripheralStateDisconnected) {
        [self throwBasedOnError:task_.error withFormat:@"Peripheral Disconnection"];
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
    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    // Check that the characteristic supports this feature.
    if ((characteristic.properties & CBCharacteristicPropertyRead) == 0) {
        NSLog(@"Characteristic does not support read.");
        throw SimpleBLE::Exception::OperationNotSupported();
    }

    characteristic_extras_t& characteristic_extras = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)];
    ble_task_t& task = characteristic_extras.task;
    std::lock_guard<std::mutex> lg(task.lock);

    if (characteristic.isNotifying) {
        // If the characteristic is already notifying, we'll just wait for the next notification.
        @synchronized(self) {
            characteristic_extras.value.reset();
        }

        WAIT_UNTIL_FALSE(self, !characteristic_extras.value.has_value());
        return characteristic_extras.value.value();

    } else {
        // Otherwise, we'll trigger a value read and wait for the response.
        @synchronized(self) {
            task.error = nil;
            task.pending = YES;
            [self.peripheral readValueForCharacteristic:characteristic];
        }

        WAIT_UNTIL_FALSE(self, task.pending);

        if (task.error != nil) {
            [self throwBasedOnError:task.error withFormat:@"Characteristic %@ Read", characteristic.UUID];
        }

        return SimpleBLE::ByteArray((const char*)characteristic.value.bytes, characteristic.value.length);
    }
}

- (void)writeRequest:(NSString*)service_uuid characteristic_uuid:(NSString*)characteristic_uuid payload:(NSData*)payload {
    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    // Check that the characteristic supports this feature.
    if ((characteristic.properties & CBCharacteristicPropertyWrite) == 0) {
        NSLog(@"Characteristic does not support write with response.");
        throw SimpleBLE::Exception::OperationNotSupported();
    }

    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].task;
    std::lock_guard<std::mutex> lg(task.lock);

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        [self.peripheral writeValue:payload forCharacteristic:characteristic type:CBCharacteristicWriteWithResponse];
    }

    WAIT_UNTIL_FALSE(self, task.pending);

    if (task.error != nil) {
        [self throwBasedOnError:task.error withFormat:@"Characteristic %@ Write Request", characteristic.UUID];
    }
}

- (void)writeCommand:(NSString*)service_uuid characteristic_uuid:(NSString*)characteristic_uuid payload:(NSData*)payload {
    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    // Check that the characteristic supports this feature.
    if ((characteristic.properties & CBCharacteristicPropertyWriteWithoutResponse) == 0) {
        NSLog(@"Characteristic does not support write without response.");
        throw SimpleBLE::Exception::OperationNotSupported();
    }

    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].task;
    std::lock_guard<std::mutex> lg(task.lock);

    // NOTE: This write is unacknowledged.
    @synchronized(self) {
        [self.peripheral writeValue:payload forCharacteristic:characteristic type:CBCharacteristicWriteWithoutResponse];
    }
}

- (void)notify:(NSString*)service_uuid
    characteristic_uuid:(NSString*)characteristic_uuid
               callback:(std::function<void(SimpleBLE::ByteArray)>)callback {
    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    characteristic_extras_t& characteristic_extras = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)];
    ble_task_t& task = characteristic_extras.task;
    std::lock_guard<std::mutex> lg(task.lock);

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        characteristic_extras.valueChangedCallback = callback;
        [self.peripheral setNotifyValue:YES forCharacteristic:characteristic];
    }

    WAIT_UNTIL_FALSE(self, task.pending);

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
    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;

    characteristic_extras_t& characteristic_extras = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)];
    ble_task_t& task = characteristic_extras.task;
    std::lock_guard<std::mutex> lg(task.lock);

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        characteristic_extras.valueChangedCallback = nil;
        [self.peripheral setNotifyValue:NO forCharacteristic:characteristic];
    }

    WAIT_UNTIL_FALSE(self, task.pending);

    if (characteristic.isNotifying || task.error != nil) {
        [self throwBasedOnError:task.error withFormat:@"Characteristic %@ Unsubscribe", characteristic.UUID];
    }
}

- (SimpleBLE::ByteArray)read:(NSString*)service_uuid
         characteristic_uuid:(NSString*)characteristic_uuid
             descriptor_uuid:(NSString*)descriptor_uuid {
    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;
    CBDescriptor* descriptor = [self findDescriptor:descriptor_uuid characteristic:characteristic];

    characteristic_extras_t& characteristic_extras = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)];
    descriptor_extras_t& descriptor_extras = characteristic_extras.descriptor_extras[uuidToSimpleBLE(descriptor.UUID)];
    ble_task_t& task = descriptor_extras.task;
    std::lock_guard<std::mutex> lg(task.lock);

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        [self.peripheral readValueForDescriptor:descriptor];
    }

    WAIT_UNTIL_FALSE(self, task.pending);

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
    std::pair<CBService*, CBCharacteristic*> serviceAndCharacteristic = [self findServiceAndCharacteristic:service_uuid
                                                                                       characteristic_uuid:characteristic_uuid];

    CBCharacteristic* characteristic = serviceAndCharacteristic.second;
    CBDescriptor* descriptor = [self findDescriptor:descriptor_uuid characteristic:characteristic];

    characteristic_extras_t& characteristic_extras = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)];
    descriptor_extras_t& descriptor_extras = characteristic_extras.descriptor_extras[uuidToSimpleBLE(descriptor.UUID)];
    ble_task_t& task = descriptor_extras.task;
    std::lock_guard<std::mutex> lg(task.lock);

    @synchronized(self) {
        task.error = nil;
        task.pending = YES;
        [self.peripheral writeValue:payload forDescriptor:descriptor];
    }

    WAIT_UNTIL_FALSE(self, task.pending);

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
        self->task_.pending = NO;
    }
}

- (void)delegateDidFailToConnect:(NSError*)error {
    @synchronized(self) {
        self->task_.error = error;
        self->task_.pending = NO;
    }
}

- (void)delegateDidDisconnect:(NSError*)error {
    @synchronized(self) {
        self->disconnectionError_ = error;
        self->task_.pending = NO;

        for (auto& characteristic_entry : self->characteristic_extras_) {
            characteristic_extras_t& characteristic_extra = characteristic_entry.second;
            characteristic_extra.task.pending = NO;

            for (auto& descriptor_entry : characteristic_extra.descriptor_extras) {
                descriptor_extras_t& descriptor_extra = descriptor_entry.second;
                descriptor_extra.task.pending = NO;
            }
        }
    }
}

#pragma mark - CBPeripheralDelegate

- (void)peripheral:(CBPeripheral*)peripheral didModifyServices:(NSArray<CBService*>*)invalidatedServices {
    // NOTE: Whenever this method is called, any pending operations are cancelled. In addition to that,
    //       the provided list of services does NOT include any characteristics or descriptors, so need to
    //       clear pending flags for those as well.
    @synchronized(self) {
        for (auto& characteristic_entry : self->characteristic_extras_) {
            characteristic_extras_t& characteristic_extra = characteristic_entry.second;
            characteristic_extra.task.pending = NO;

            for (auto& descriptor_entry : characteristic_extra.descriptor_extras) {
                descriptor_extras_t& descriptor_extra = descriptor_entry.second;
                descriptor_extra.task.pending = NO;
            }
        }
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didDiscoverServices:(NSError*)error {
    @synchronized(self) {
        self->task_.error = error;
        self->task_.pending = NO;
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didDiscoverCharacteristicsForService:(CBService*)service error:(NSError*)error {
    @synchronized(self) {
        self->task_.error = error;
        self->task_.pending = NO;
    }
}

- (void)peripheral:(CBPeripheral*)peripheral
    didDiscoverDescriptorsForCharacteristic:(CBCharacteristic*)characteristic
                                      error:(NSError*)error {
    @synchronized(self) {
        self->task_.error = error;
        self->task_.pending = NO;
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didUpdateValueForCharacteristic:(CBCharacteristic*)characteristic error:(NSError*)error {
    characteristic_extras_t& characteristic_extra = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)];

    if (characteristic.isNotifying) {
        // If the characteristic is notifying, just save the value and trigger the callback.
        @synchronized(self) {
            characteristic_extra.value = SimpleBLE::ByteArray((const char*)characteristic.value.bytes, characteristic.value.length);
        }

        if (characteristic_extra.valueChangedCallback != nil) {
            characteristic_extra.valueChangedCallback(characteristic_extra.value.value());
        }

    } else {
        // If the characteristic is not notifying, then this is a response to a read request.
        ble_task_t& task = characteristic_extra.task;
        @synchronized(self) {
            task.error = error;
            if (task.pending) {
                task.pending = NO;
            }
        }
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didWriteValueForCharacteristic:(CBCharacteristic*)characteristic error:(NSError*)error {
    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].task;

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
    ble_task_t& task = characteristic_extras_[uuidToSimpleBLE(characteristic.UUID)].task;

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
    const std::string characteristic_uuid = uuidToSimpleBLE(descriptor.characteristic.UUID);
    const std::string descriptor_uuid = uuidToSimpleBLE(descriptor.UUID);
    ble_task_t& task = characteristic_extras_[characteristic_uuid].descriptor_extras[descriptor_uuid].task;

    @synchronized(self) {
        task.error = error;
        if (task.pending) {
            task.pending = NO;
        }
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didWriteValueForDescriptor:(CBDescriptor*)descriptor error:(NSError*)error {
    const std::string characteristic_uuid = uuidToSimpleBLE(descriptor.characteristic.UUID);
    const std::string descriptor_uuid = uuidToSimpleBLE(descriptor.UUID);
    ble_task_t& task = characteristic_extras_[characteristic_uuid].descriptor_extras[descriptor_uuid].task;

    @synchronized(self) {
        task.error = error;
        if (task.pending) {
            task.pending = NO;
        }
    }
}

@end
