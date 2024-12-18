#pragma once

#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>

#include <functional>
#import "AdapterMac.h"

@interface AdapterBaseMacOS : NSObject<CBCentralManagerDelegate>

@property NSString* uuid;

- (bool)isBluetoothEnabled;

- (instancetype)init:(SimpleBLE::AdapterMac*)adapter;

- (void*)underlying;

- (void)scanStart;

- (void)scanStop;

- (bool)scanIsActive;

- (NSString*)address;

@end
