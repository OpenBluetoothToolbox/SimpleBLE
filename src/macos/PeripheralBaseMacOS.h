#pragma once

#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>

#include <functional>
#include "PeripheralBase.h"

@interface PeripheralBaseMacOS : NSObject<CBPeripheralDelegate>

- (instancetype)init:(CBPeripheral *)peripheral;

- (NSString *) identifier;

- (NSString *) address;

@end
