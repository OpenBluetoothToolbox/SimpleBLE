// This weird pragma is required for the compiler to properly include the necessary namespaces.
#pragma comment(lib, "windowsapp")

#include "PeripheralBase.h"
#include "Utils.h"

#include "winrt/Windows.Foundation.h"
#include "winrt/base.h"

#include <iostream>

using namespace SimpleBLE;

PeripheralBase::PeripheralBase(advertising_data_t advertising_data) {
    identifier_ = advertising_data.identifier;
    address_ = advertising_data.mac_address;
    device_ = BluetoothLEDevice::FromBluetoothAddressAsync(_str_to_mac_address(advertising_data.mac_address)).get();
}

PeripheralBase::~PeripheralBase() {}

std::string PeripheralBase::identifier() {
    return identifier_;
}

BluetoothAddress PeripheralBase::address() {
    return address_;
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
