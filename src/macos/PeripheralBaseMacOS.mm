#import "PeripheralBaseMacOS.h"

@interface PeripheralBaseMacOS () {
}

// Private properties
@property(strong) CBPeripheral* peripheral;
@property(strong) CBCentralManager* centralManager;

@end

@implementation PeripheralBaseMacOS

- (instancetype)init:(CBPeripheral*)peripheral centralManager:(CBCentralManager*)centralManager {
    self = [super init];
    if (self) {
        // NOTE: It's important to make a copy of the peripheral and central objects into
        // a strong property to prevent them from being deallocated by the garbage collector.
        _peripheral = [peripheral copy];
        _centralManager = centralManager;

        _peripheral.delegate = self;
    }
    return self;
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

- (void)connect {
    //NSLog(@"Connecting to peripheral: %@", self.peripheral.name);
    [self.centralManager connectPeripheral:self.peripheral options:@{}];  // TODO: Do we need to pass any options?

    NSDate* endDate = nil;

    // Wait for the connection to be established for up to 5 seconds.
    endDate = [NSDate dateWithTimeInterval:5.0 sinceDate:NSDate.now];
    while (self.peripheral.state == CBPeripheralStateConnecting && [NSDate.now compare:endDate] == NSOrderedAscending) {
        [NSThread sleepForTimeInterval:0.01];
    }

    if (self.peripheral.state != CBPeripheralStateConnected) {
        // If the connection failed, raise an exception.
        // TODO: Raise an exception.
        NSLog(@"Connection failed.");
        return;
    }

    [self.peripheral discoverServices:nil];

    // Wait for services to be discovered for up to 1 second.
    // NOTE: This is a bit of a hack but avoids the need of having a dedicated flag.
    endDate = [NSDate dateWithTimeInterval:1.0 sinceDate:NSDate.now];
    while (self.peripheral.services == nil && [NSDate.now compare:endDate] == NSOrderedAscending) {
        [NSThread sleepForTimeInterval:0.01];
    }

    if (self.peripheral.services == nil) {
        // If services could not be discovered, raise an exception.
        // TODO: Raise an exception.
        NSLog(@"Services could not be discovered.");
        return;
    }

    // For each service found, discover characteristics.
    for (CBService* service in self.peripheral.services) {
        [self.peripheral discoverCharacteristics:nil forService:service];

        // Wait for characteristics  to be discovered for up to 1 second.
        // NOTE: This is a bit of a hack but avoids the need of having a dedicated flag.
        endDate = [NSDate dateWithTimeInterval:1.0 sinceDate:NSDate.now];
        while (service.characteristics == nil && [NSDate.now compare:endDate] == NSOrderedAscending) {
            [NSThread sleepForTimeInterval:0.01];
        }

        if (service.characteristics == nil) {
            // If characteristics could not be discovered, raise an exception.
            // TODO: Raise an exception.
            NSLog(@"Characteristics could not be discovered for service %@", service.UUID);
        }
    }
}

- (void)disconnect {
    //NSLog(@"Disconnecting peripheral: %@ - State was %ld", self.peripheral.name, self.peripheral.state);
    [self.centralManager cancelPeripheralConnection:self.peripheral];

    NSDate* endDate = nil;

    // Wait for the connection to be established for up to 5 seconds.
    endDate = [NSDate dateWithTimeInterval:5.0 sinceDate:NSDate.now];
    while (self.peripheral.state == CBPeripheralStateDisconnecting && [NSDate.now compare:endDate] == NSOrderedAscending) {
        [NSThread sleepForTimeInterval:0.01];
    }

    if (self.peripheral.state != CBPeripheralStateDisconnected) {
        // If the disconnection failed, raise an exception.
        // TODO: Raise an exception.
        NSLog(@"Disconnection failed.");
        return;
    }
}

- (bool)isConnected {
    return self.peripheral.state == CBPeripheralStateConnected;
}

- (std::vector<SimpleBLE::BluetoothService>)getServices {
    // NOTE: We might want to return NSUUIDs in this function and convert them to
    // strings in the PeripheralBase class.

    std::vector<SimpleBLE::BluetoothService> services;

    // For each service, load the UUID and the corresponding characteristics.
    for (CBService* service in self.peripheral.services) {
        SimpleBLE::BluetoothService bluetoothService;
        bluetoothService.uuid = [[service.UUID UUIDString] UTF8String];

        // Load all the characteristics for this service.
        NSArray<CBCharacteristic*>* characteristics = service.characteristics;
        for (CBCharacteristic* characteristic in characteristics) {
            bluetoothService.characteristics.push_back([[characteristic.UUID UUIDString] UTF8String]);
        }

        services.push_back(bluetoothService);
    }

    return services;
}

#pragma mark - CBCentralManagerDelegate

- (void)delegateDidConnect {
    // NOTE: As the connection process is polling-based, this callback is not needed,
    // but might be useful in the future.
}

- (void)delegateDidDisconnect {
    // NOTE: We're keeping this callback for potential future use.
}

#pragma mark - CBPeripheralDelegate

- (void)peripheral:(CBPeripheral*)peripheral didDiscoverServices:(NSError*)error {
    // NOTE: As we are currently polling the result of the discovery, this callback is not needed,
    // but might be useful in the future.
    if (error != nil) {
        NSLog(@"Error while discovering services for peripheral %@: %@\n", peripheral.name, error);
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didDiscoverCharacteristicsForService:(CBService*)service error:(NSError*)error {
    // NOTE: As we are currently polling the result of the discovery, this callback is not needed,
    // but might be useful in the future.
    if (error != nil) {
        NSLog(@"Error while discovering characteristics for service %@: %@\n", service.UUID, error);
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didUpdateValueForCharacteristic:(CBCharacteristic*)characteristic error:(NSError*)error {
    NSLog(@"Updated value for characteristic: %@", characteristic.UUID);
    if (error != nil) {
        NSLog(@"Error: %@\n", error);
    }
}

- (void)peripheral:(CBPeripheral*)peripheral didWriteValueForCharacteristic:(CBCharacteristic*)characteristic error:(NSError*)error {
    NSLog(@"Wrote value for characteristic: %@", characteristic.UUID);
    if (error != nil) {
        NSLog(@"Error: %@\n", error);
    }
}

- (void)peripheral:(CBPeripheral*)peripheral
    didUpdateNotificationStateForCharacteristic:(CBCharacteristic*)characteristic
                                          error:(NSError*)error {
    NSLog(@"Updated notification state for characteristic: %@", characteristic.UUID);
    if (error != nil) {
        NSLog(@"Error: %@\n", error);
    }
}

@end
