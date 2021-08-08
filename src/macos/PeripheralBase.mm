#import "PeripheralBase.h"
#import "PeripheralBaseMacOS.h"

#include <iostream>

using namespace SimpleBLE;

PeripheralBase::PeripheralBase(void* opaque_peripheral, advertising_data_t advertising_data) {
    this->opaque_internal_ = [[PeripheralBaseMacOS alloc] init:(CBPeripheral*)opaque_peripheral];
}

PeripheralBase::~PeripheralBase() {}

std::string PeripheralBase::identifier() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    return std::string([[internal identifier] UTF8String]);
}

BluetoothAddress PeripheralBase::address() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    return std::string([[internal address] UTF8String]);
}
