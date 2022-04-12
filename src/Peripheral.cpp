#include <simpleble/Peripheral.h>

#include <simpleble/Exceptions.h>
#include "PeripheralBase.h"

using namespace SimpleBLE;

bool Peripheral::initialized() const { return internal_ != nullptr; }

// TODO: Add validations to prevent calls into internal_ if not set.

std::string Peripheral::identifier() { return internal_->identifier(); }

BluetoothAddress Peripheral::address() { return internal_->address(); }

int16_t Peripheral::rssi() { return internal_->rssi(); }

void Peripheral::connect() { internal_->connect(); }

void Peripheral::disconnect() { internal_->disconnect(); }

bool Peripheral::is_connected() { return internal_->is_connected(); }

bool Peripheral::is_connectable() { return internal_->is_connectable(); }

bool Peripheral::is_paired() { return internal_->is_paired(); }

void Peripheral::unpair() { internal_->unpair(); }

std::vector<BluetoothService> Peripheral::services() {
    if (!is_connected()) {
        throw Exception::OperationFailed();
    }

    return internal_->services();
}

std::map<uint16_t, ByteArray> Peripheral::manufacturer_data() { return internal_->manufacturer_data(); }

ByteArray Peripheral::read(BluetoothUUID const& service, BluetoothUUID const& characteristic) {
    if (!is_connected()) {
        throw Exception::OperationFailed();
    }

    return internal_->read(service, characteristic);
}

void Peripheral::write_request(BluetoothUUID const& service, BluetoothUUID const& characteristic, ByteArray const& data) {
    if (!is_connected()) {
        throw Exception::OperationFailed();
    }

    internal_->write_request(service, characteristic, data);
}

void Peripheral::write_command(BluetoothUUID const& service, BluetoothUUID const& characteristic, ByteArray const& data) {
    if (!is_connected()) {
        throw Exception::OperationFailed();
    }

    internal_->write_command(service, characteristic, data);
}

void Peripheral::notify(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                        std::function<void(ByteArray payload)> callback) {
    if (!is_connected()) {
        throw Exception::OperationFailed();
    }

    internal_->notify(service, characteristic, std::move(callback));
}

void Peripheral::indicate(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                          std::function<void(ByteArray payload)> callback) {
    if (!is_connected()) {
        throw Exception::OperationFailed();
    }

    internal_->indicate(service, characteristic, std::move(callback));
}

void Peripheral::unsubscribe(BluetoothUUID const& service, BluetoothUUID const& characteristic) {
    if (!is_connected()) {
        throw Exception::OperationFailed();
    }

    internal_->unsubscribe(service, characteristic);
}

void Peripheral::set_callback_on_connected(std::function<void()> on_connected) {
    internal_->set_callback_on_connected(std::move(on_connected));
}

void Peripheral::set_callback_on_disconnected(std::function<void()> on_disconnected) {
    internal_->set_callback_on_disconnected(std::move(on_disconnected));
}
