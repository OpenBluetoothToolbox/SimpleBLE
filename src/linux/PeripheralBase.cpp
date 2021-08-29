#include "PeripheralBase.h"
#include "Bluez.h"

#include <iostream>

using namespace SimpleBLE;

PeripheralBase::PeripheralBase(std::shared_ptr<BluezDevice> device) { device_ = device; }

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

void PeripheralBase::connect() {}

void PeripheralBase::disconnect() {}

bool PeripheralBase::is_connected() { return false; }

std::vector<BluetoothService> PeripheralBase::services() { return {}; }

ByteArray PeripheralBase::read(BluetoothUUID service, BluetoothUUID characteristic) { return ""; }

void PeripheralBase::write_request(BluetoothUUID service, BluetoothUUID characteristic, ByteArray data) {}

void PeripheralBase::write_command(BluetoothUUID service, BluetoothUUID characteristic, ByteArray data) {}

void PeripheralBase::notify(BluetoothUUID service, BluetoothUUID characteristic,
                            std::function<void(ByteArray payload)> callback) {}

void PeripheralBase::indicate(BluetoothUUID service, BluetoothUUID characteristic,
                              std::function<void(ByteArray payload)> callback) {}

void PeripheralBase::unsubscribe(BluetoothUUID service, BluetoothUUID characteristic) {}

void PeripheralBase::set_callback_on_connected(std::function<void()> on_connected) {}

void PeripheralBase::set_callback_on_disconnected(std::function<void()> on_disconnected) {}
