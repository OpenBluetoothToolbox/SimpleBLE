#include <simpleble/Peripheral.h>

#include <simpleble/Exceptions.h>
#include "PeripheralBase.h"

using namespace SimpleBLE;

bool Peripheral::initialized() const { return internal_ != nullptr; }

void* Peripheral::underlying() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().underlying();
}

std::string Peripheral::identifier() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().identifier();
}

SimpleBLE::BluetoothAddress Peripheral::address() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().address();
}

SimpleBLE::BluetoothAddressType Peripheral::address_type() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().address_type();
}

int16_t Peripheral::rssi() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().rssi();
}

int16_t Peripheral::tx_power() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().tx_power();
}

uint16_t Peripheral::mtu() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().mtu();
}

void Peripheral::connect() {
    if (!initialized()) throw Exception::NotInitialized();

    internal().connect();
}

void Peripheral::disconnect() {
    if (!initialized()) throw Exception::NotInitialized();

    internal().disconnect();
}

bool Peripheral::is_connected() {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().is_connected();
}

bool Peripheral::is_connectable() {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().is_connectable();
}

bool Peripheral::is_paired() {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().is_paired();
}

void Peripheral::unpair() {
    if (!initialized()) throw Exception::NotInitialized();

    internal().unpair();
}

std::vector<Service> Peripheral::services() {
    if (!initialized()) throw Exception::NotInitialized();

    if (is_connected()) {
        return internal().services();
    } else {
        return internal().advertised_services();
    }

    return internal().services();
}

std::map<uint16_t, ByteArray> Peripheral::manufacturer_data() {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().manufacturer_data();
}

ByteArray Peripheral::read(BluetoothUUID const& service, BluetoothUUID const& characteristic) {
    if (!initialized()) throw Exception::NotInitialized();
    if (!is_connected()) throw Exception::NotConnected();

    return internal().read(service, characteristic);
}

void Peripheral::write_request(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                               ByteArray const& data) {
    if (!initialized()) throw Exception::NotInitialized();
    if (!is_connected()) throw Exception::NotConnected();

    internal().write_request(service, characteristic, data);
}

void Peripheral::write_command(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                               ByteArray const& data) {
    if (!initialized()) throw Exception::NotInitialized();
    if (!is_connected()) throw Exception::NotConnected();

    internal().write_command(service, characteristic, data);
}

void Peripheral::notify(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                        std::function<void(ByteArray payload)> callback) {
    if (!initialized()) throw Exception::NotInitialized();
    if (!is_connected()) throw Exception::NotConnected();

    internal().notify(service, characteristic, std::move(callback));
}

void Peripheral::indicate(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                          std::function<void(ByteArray payload)> callback) {
    if (!initialized()) throw Exception::NotInitialized();
    if (!is_connected()) throw Exception::NotConnected();

    internal().indicate(service, characteristic, std::move(callback));
}

void Peripheral::unsubscribe(BluetoothUUID const& service, BluetoothUUID const& characteristic) {
    if (!initialized()) throw Exception::NotInitialized();
    if (!is_connected()) throw Exception::NotConnected();

    internal().unsubscribe(service, characteristic);
}

ByteArray Peripheral::read(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                           BluetoothUUID const& descriptor) {
    if (!initialized()) throw Exception::NotInitialized();
    if (!is_connected()) throw Exception::NotConnected();

    return internal().read(service, characteristic, descriptor);
}

void Peripheral::write(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                       BluetoothUUID const& descriptor, ByteArray const& data) {
    if (!initialized()) throw Exception::NotInitialized();
    if (!is_connected()) throw Exception::NotConnected();

    internal().write(service, characteristic, descriptor, data);
}

void Peripheral::set_callback_on_connected(std::function<void()> on_connected) {
    if (!initialized()) throw Exception::NotInitialized();

    internal().set_callback_on_connected(std::move(on_connected));
}

void Peripheral::set_callback_on_disconnected(std::function<void()> on_disconnected) {
    if (!initialized()) throw Exception::NotInitialized();

    internal().set_callback_on_disconnected(std::move(on_disconnected));
}

PeripheralBase &Peripheral::internal() {
    return *internal_;
}

const PeripheralBase &Peripheral::internal() const {
    return *internal_;
}
