#pragma once

#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>

#include <functional>
#include "AdapterBase.h"

@interface AdapterBaseMacOS : NSObject<CBCentralManagerDelegate>

- (instancetype)init:(SimpleBLE::AdapterBase*)adapter;

- (void*)underlying;

- (void)scanStart;
- (void)scanStartWithServices:(NSArray<NSString*>*)serviceUUIDs;

- (void)scanStop;

- (bool)scanIsActive;

@end
