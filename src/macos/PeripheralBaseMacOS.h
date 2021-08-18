#pragma once

#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>

#include <functional>
#include <vector>
#include "PeripheralBase.h"

@interface PeripheralBaseMacOS : NSObject<CBPeripheralDelegate>

- (instancetype)init:(CBPeripheral*)peripheral centralManager:(CBCentralManager*)centralManager;

- (NSString*)identifier;
- (NSString*)address;

- (void)connect;
- (void)disconnect;
- (bool)isConnected;
- (std::vector<SimpleBLE::BluetoothService>)getServices;

- (void)delegateDidConnect;
- (void)delegateDidDisconnect;

@end
