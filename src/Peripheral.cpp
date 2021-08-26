#include <simpleble/Peripheral.h>

#include "PeripheralBase.h"

#include <iostream>

using namespace SimpleBLE;

Peripheral::Peripheral() {}

Peripheral::~Peripheral() {}

// TODO: Add validations to prevent calls into internal_ if not set.

std::string Peripheral::identifier() { return internal_->identifier(); }

BluetoothAddress Peripheral::address() { return internal_->address(); }

void Peripheral::connect() { return internal_->connect(); }

void Peripheral::disconnect() { return internal_->disconnect(); }

bool Peripheral::is_connected() { return internal_->is_connected(); }

std::vector<BluetoothService> Peripheral::services() { return internal_->services(); }

ByteArray Peripheral::read(BluetoothUUID service, BluetoothUUID characteristic) {
    return internal_->read(service, characteristic);
}

void Peripheral::write_request(BluetoothUUID service, BluetoothUUID characteristic, ByteArray data) {
    internal_->write_request(service, characteristic, data);
}

void Peripheral::write_command(BluetoothUUID service, BluetoothUUID characteristic, ByteArray data) {
    internal_->write_command(service, characteristic, data);
}

void Peripheral::notify(BluetoothUUID service, BluetoothUUID characteristic,
                        std::function<void(ByteArray payload)> callback) {
    internal_->notify(service, characteristic, callback);
}

void Peripheral::indicate(BluetoothUUID service, BluetoothUUID characteristic,
                          std::function<void(ByteArray payload)> callback) {
    internal_->indicate(service, characteristic, callback);
}

void Peripheral::unsubscribe(BluetoothUUID service, BluetoothUUID characteristic) {
    internal_->unsubscribe(service, characteristic);
}

void Peripheral::set_callback_on_connected(std::function<void()> on_connected) {
    internal_->set_callback_on_connected(on_connected);
}

void Peripheral::set_callback_on_disconnected(std::function<void()> on_disconnected) {
    internal_->set_callback_on_disconnected(on_disconnected);
}
