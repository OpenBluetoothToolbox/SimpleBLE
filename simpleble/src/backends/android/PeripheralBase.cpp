#include "PeripheralBase.h"

#include "CharacteristicBuilder.h"
#include "DescriptorBuilder.h"
#include "ServiceBuilder.h"

#include <simpleble/Exceptions.h>
#include <algorithm>
#include "CommonUtils.h"
#include "LoggingInternal.h"

#include <android/log.h>

using namespace SimpleBLE;
using namespace std::chrono_literals;



PeripheralBase::PeripheralBase(Android::ScanResult scan_result) : _device(scan_result.getDevice()) {
    _btGattCallback.set_callback_onConnectionStateChange([this](bool connected) {
        // If a connection has been established, request service discovery.
        if (connected) {
            _gatt.discoverServices();
        } else {
            // TODO: Whatever cleanup is necessary when disconnected.
            __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", "Disconnected from device");
            SAFE_CALLBACK_CALL(callback_on_disconnected_);
        }
    });

    _btGattCallback.set_callback_onServicesDiscovered([this]() {
        std::vector<Android::BluetoothGattService> services = _gatt.getServices();

        auto msg = fmt::format("Services discovered: {}", services.size());
        __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg.c_str());

        for (auto& service : services) {
            auto msg = fmt::format("Service: {}", service.getUuid());
            __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg.c_str());
        }

        // TODO KEVIN: RETRIEVE SERVICES AND CHARACTERISTICS

        // Notify the user that the connection has been established once services hace been discovered.
        SAFE_CALLBACK_CALL(callback_on_connected_);
    });

}

PeripheralBase::~PeripheralBase() {}

void PeripheralBase::update_advertising_data(Android::ScanResult scan_result) {}

void* PeripheralBase::underlying() const { return nullptr; }

std::string PeripheralBase::identifier() { return _device.getName(); }

BluetoothAddress PeripheralBase::address() { return BluetoothAddress(_device.getAddress()); }

BluetoothAddressType PeripheralBase::address_type() { return BluetoothAddressType::UNSPECIFIED; }

int16_t PeripheralBase::rssi() { return 0; }

int16_t PeripheralBase::tx_power() { return 0; }

uint16_t PeripheralBase::mtu() { return _btGattCallback.mtu; }

void PeripheralBase::connect() {
    _gatt = _device.connectGatt(false, _btGattCallback);
}

void PeripheralBase::disconnect() {
    _gatt.disconnect();
}

bool PeripheralBase::is_connected() { return _btGattCallback.connected && _btGattCallback.services_discovered; }

bool PeripheralBase::is_connectable() { return false; }

bool PeripheralBase::is_paired() { return false; }

void PeripheralBase::unpair() {}

std::vector<Service> PeripheralBase::services() { return std::vector<Service>(); }

std::vector<Service> PeripheralBase::advertised_services() { return std::vector<Service>(); }

std::map<uint16_t, ByteArray> PeripheralBase::manufacturer_data() { return std::map<uint16_t, ByteArray>(); }

ByteArray PeripheralBase::read(BluetoothUUID const& service, BluetoothUUID const& characteristic) {
    return ByteArray();
}

void PeripheralBase::write_request(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                                   ByteArray const& data) {}

void PeripheralBase::write_command(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                                   ByteArray const& data) {}

void PeripheralBase::notify(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                            std::function<void(ByteArray payload)> callback) {}

void PeripheralBase::indicate(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                              std::function<void(ByteArray payload)> callback) {}

void PeripheralBase::unsubscribe(BluetoothUUID const& service, BluetoothUUID const& characteristic) {}

ByteArray PeripheralBase::read(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                               BluetoothUUID const& descriptor) {
    return ByteArray();
}

void PeripheralBase::write(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                           BluetoothUUID const& descriptor, ByteArray const& data) {}

void PeripheralBase::set_callback_on_connected(std::function<void()> on_connected) {
    if (on_connected) {
        callback_on_connected_.load(std::move(on_connected));
    } else {
        callback_on_connected_.unload();
    }
}

void PeripheralBase::set_callback_on_disconnected(std::function<void()> on_disconnected) {
    if (on_disconnected) {
        callback_on_disconnected_.load(std::move(on_disconnected));
    } else {
        callback_on_disconnected_.unload();
    }
}
