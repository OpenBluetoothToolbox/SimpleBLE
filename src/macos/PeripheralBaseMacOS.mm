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
    NSLog(@"Connecting to peripheral: %@", self.peripheral.name);
    [self.centralManager connectPeripheral:self.peripheral options:@{}]; // TODO: Do we need to pass any options?

    // Wait for the connection to be established.


    // // Validate the central manager state by checking if it is powered on for up to 5 seconds.
    // NSDate* endDate = [NSDate dateWithTimeInterval:5.0 sinceDate:NSDate.now];
    // while (self.centralManager.state != CBManagerStatePoweredOn && [NSDate.now compare:endDate] == NSOrderedAscending) {
    //     [NSThread sleepForTimeInterval:0.01];
    // }    
}

- (void)disconnect {
    NSLog(@"Disconnecting peripheral: %@ - State was %ld", self.peripheral.name, self.peripheral.state);
    [self.centralManager cancelPeripheralConnection:self.peripheral];
}

- (void)delegateDidConnect {

}

- (void)delegateDidDiscoonnect {
    
}

@end
