#pragma once

#include <simpleble/Exceptions.h>
#include <simpleble/Service.h>
#include <simpleble/Types.h>

#include <kvn_safe_callback.hpp>

#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>

namespace SimpleBLE {

class PeripheralBase {
  public:
    PeripheralBase() = default;
    virtual ~PeripheralBase() = default;

    virtual void* underlying() const = 0;

    virtual std::string identifier() = 0;
    virtual BluetoothAddress address() = 0;
    virtual BluetoothAddressType address_type() = 0;
    virtual int16_t rssi() = 0;
    virtual int16_t tx_power() = 0;
    virtual uint16_t mtu() = 0;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual bool is_connected() = 0;
    virtual bool is_connectable() = 0;
    virtual bool is_paired() = 0;
    virtual void unpair() = 0;

    virtual std::vector<Service> services() = 0;
    virtual std::vector<Service> advertised_services() = 0;
    virtual std::map<uint16_t, ByteArray> manufacturer_data() = 0;

    // clang-format off
    virtual ByteArray read(BluetoothUUID const& service, BluetoothUUID const& characteristic) = 0;
    virtual void write_request(BluetoothUUID const& service, BluetoothUUID const& characteristic, ByteArray const& data) = 0;
    virtual void write_command(BluetoothUUID const& service, BluetoothUUID const& characteristic, ByteArray const& data) = 0;
    virtual void notify(BluetoothUUID const& service, BluetoothUUID const& characteristic, std::function<void(ByteArray payload)> callback) = 0;
    virtual void indicate(BluetoothUUID const& service, BluetoothUUID const& characteristic, std::function<void(ByteArray payload)> callback) = 0;
    virtual void unsubscribe(BluetoothUUID const& service, BluetoothUUID const& characteristic) = 0;

    virtual ByteArray read(BluetoothUUID const& service, BluetoothUUID const& characteristic, BluetoothUUID const& descriptor) = 0;
    virtual void write(BluetoothUUID const& service, BluetoothUUID const& characteristic, BluetoothUUID const& descriptor, ByteArray const& data) = 0;
    // clang-format on

    virtual void set_callback_on_connected(std::function<void()> on_connected) = 0;
    virtual void set_callback_on_disconnected(std::function<void()> on_disconnected) = 0;

};

}  // namespace SimpleBLE
