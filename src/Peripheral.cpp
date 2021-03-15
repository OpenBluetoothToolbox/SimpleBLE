#include <simpleble/Peripheral.h>

#include "PeripheralBase.h"

#include <iostream>

using namespace SimpleBLE;

Peripheral::Peripheral() {}

Peripheral::~Peripheral() {}

// TODO: Add validations to prevent calls into internal_ if not set.

std::string Peripheral::identifier() {
    return internal_->identifier();
}

BluetoothAddress Peripheral::address() {
    return internal_->address();
}