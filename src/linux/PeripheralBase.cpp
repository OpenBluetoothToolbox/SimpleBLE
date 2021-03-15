#include "PeripheralBase.h"
#include "Bluez.h"

#include <iostream>

using namespace SimpleBLE;

PeripheralBase::PeripheralBase(std::shared_ptr<BluezDevice> device) {
    device_ = device;
}

PeripheralBase::~PeripheralBase() {}

std::string PeripheralBase::identifier() {
    auto device = device_.lock();
    if (device) {
        return device->get_name();
    } else {
        throw Exception::InvalidReference();
    }
}

BluetoothAddress PeripheralBase::address() {
    auto device = device_.lock();
    if (device) {
        return device->get_address();
    } else {
        throw Exception::InvalidReference();
    }
}
