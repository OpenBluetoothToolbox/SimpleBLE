#import "PeripheralBaseMacOS.h"

@interface PeripheralBaseMacOS () {
}

// Private properties
@property(strong) CBPeripheral* peripheral;

@end

@implementation PeripheralBaseMacOS

- (instancetype)init:(CBPeripheral*)peripheral {
    self = [super init];
    if (self) {
        _peripheral = [peripheral copy]; // It's important to make a copy of the peripheral object into 
        // a strong property to prevent it from being deallocated by the garbage collector.
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

@end
