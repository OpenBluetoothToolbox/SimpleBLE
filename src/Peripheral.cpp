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

void Peripheral::connect() {
    return internal_->connect();
}

void Peripheral::disconnect() {
    return internal_->disconnect();
}

void Peripheral::set_callback_on_connected(std::function<void()> on_connected) {
    internal_->set_callback_on_connected(on_connected);
}

void Peripheral::set_callback_on_disconnected(std::function<void()> on_disconnected) {
    internal_->set_callback_on_disconnected(on_disconnected);
}
