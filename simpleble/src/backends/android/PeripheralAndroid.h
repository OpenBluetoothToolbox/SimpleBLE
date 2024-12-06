#pragma once

#include <simpleble/Exceptions.h>
#include <simpleble/Service.h>
#include <simpleble/Types.h>

#include <kvn_safe_callback.hpp>
#include "../common/PeripheralBase.h"

#include <android/ScanResult.h>
#include <bridge/BluetoothGattCallback.h>

#include <map>

namespace SimpleBLE {

class PeripheralAndroid : public PeripheralBase {
  public:
    PeripheralAndroid(Android::ScanResult scan_result);
    virtual ~PeripheralAndroid();

    virtual void* underlying() const override;

    virtual std::string identifier() override;
    virtual BluetoothAddress address() override;
    virtual BluetoothAddressType address_type() override;
    virtual int16_t rssi() override;
    virtual int16_t tx_power() override;
    virtual uint16_t mtu() override;

    virtual void connect() override;
    virtual void disconnect() override;
    virtual bool is_connected() override;
    virtual bool is_connectable() override;
    virtual bool is_paired() override;
    virtual void unpair() override;

    virtual std::vector<std::shared_ptr<ServiceBase>> available_services() override;
    virtual std::vector<std::shared_ptr<ServiceBase>> advertised_services() override;
    virtual std::map<uint16_t, ByteArray> manufacturer_data() override;

    // clang-format off
    virtual ByteArray read_inner(BluetoothUUID const& service, BluetoothUUID const& characteristic) override;
    virtual void write_request_inner(BluetoothUUID const& service, BluetoothUUID const& characteristic, ByteArray const& data) override;
    virtual void write_command_inner(BluetoothUUID const& service, BluetoothUUID const& characteristic, ByteArray const& data) override;
    virtual void notify_inner(BluetoothUUID const& service, BluetoothUUID const& characteristic, std::function<void(ByteArray payload)> callback) override;
    virtual void indicate_inner(BluetoothUUID const& service, BluetoothUUID const& characteristic, std::function<void(ByteArray payload)> callback) override;
    virtual void unsubscribe_inner(BluetoothUUID const& service, BluetoothUUID const& characteristic) override;

    virtual ByteArray read_inner(BluetoothUUID const& service, BluetoothUUID const& characteristic, BluetoothUUID const& descriptor) override;
    virtual void write_inner(BluetoothUUID const& service, BluetoothUUID const& characteristic, BluetoothUUID const& descriptor, ByteArray const& data) override;
    // clang-format on

    virtual void set_callback_on_connected(std::function<void()> on_connected) override;
    virtual void set_callback_on_disconnected(std::function<void()> on_disconnected) override;

    void update_advertising_data(Android::ScanResult scan_result);

  private:
    Android::Bridge::BluetoothGattCallback _btGattCallback;
    Android::BluetoothDevice _device;
    Android::BluetoothGatt _gatt;
    std::vector<Android::BluetoothGattService> _services;

    kvn::safe_callback<void()> callback_on_connected_;
    kvn::safe_callback<void()> callback_on_disconnected_;

    Android::BluetoothGattCharacteristic _fetch_characteristic(const BluetoothUUID& service_uuid,
                                                               const BluetoothUUID& characteristic_uuid);
    Android::BluetoothGattDescriptor _fetch_descriptor(const BluetoothUUID& service_uuid,
                                                       const BluetoothUUID& characteristic_uuid,
                                                       const BluetoothUUID& descriptor_uuid);
};

}  // namespace SimpleBLE
