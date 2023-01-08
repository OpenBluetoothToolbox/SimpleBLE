#pragma once

#include "rust/cxx.h"
#include "simpleble/Adapter.h"
#include "simpleble/Peripheral.h"

#include <cstdint>
#include <iostream>
#include <memory>

namespace SimpleRsBLE {

struct Adapter;
struct Peripheral;

};  // namespace SimpleRsBLE

namespace SimpleBLE {

struct RustyAdapterWrapper;
struct RustyPeripheralWrapper;
struct RustyServiceWrapper;
struct RustyCharacteristicWrapper;
struct RustyDescriptorWrapper;

class RustyAdapter : private Adapter {
  public:
    RustyAdapter() = default;
    virtual ~RustyAdapter() = default;

    RustyAdapter(Adapter adapter)
        : _internal(new Adapter(adapter)), _adapter(std::make_unique<SimpleRsBLE::Adapter*>()){};

    void link(SimpleRsBLE::Adapter& target) const;
    void unlink() const;

    rust::String identifier() const;
    rust::String address() const;

    void scan_start() const;
    void scan_stop() const;
    void scan_for(int32_t timeout_ms) const;
    bool scan_is_active() const;
    rust::Vec<SimpleBLE::RustyPeripheralWrapper> scan_get_results() const;

    rust::Vec<SimpleBLE::RustyPeripheralWrapper> get_paired_peripherals() const;

  private:
    // NOTE: All internal properties need to be handled as pointers,
    // allowing the calls to RustyAdapter to always be const.
    // This might require us to store pointers to pointers, so it's
    // important to be careful when handling these.
    std::shared_ptr<Adapter> _internal;
    std::unique_ptr<SimpleRsBLE::Adapter*> _adapter;
};

class RustyPeripheral : private Peripheral {
  public:
    RustyPeripheral() = default;
    virtual ~RustyPeripheral() = default;

    RustyPeripheral(Peripheral peripheral)
        : _internal(new Peripheral(peripheral)), _peripheral(std::make_unique<SimpleRsBLE::Peripheral*>()) {}

    void link(SimpleRsBLE::Peripheral& target) const;
    void unlink() const;

    rust::String identifier() const;
    rust::String address() const;
    BluetoothAddressType address_type() const;
    int16_t rssi() const;

    int16_t tx_power() const;
    uint16_t mtu() const;

    void connect() const;
    void disconnect() const;
    bool is_connected() const;
    bool is_connectable() const;
    bool is_paired() const;
    void unpair() const;

    rust::Vec<SimpleBLE::RustyServiceWrapper> services() const;

  private:
    // NOTE: All internal properties need to be handled as pointers,
    // allowing the calls to RustyPeripheral to always be const.
    // This might require us to store pointers to pointers, so it's
    // important to be careful when handling these.
    std::shared_ptr<Peripheral> _internal;
    std::unique_ptr<SimpleRsBLE::Peripheral*> _peripheral;
};

class RustyService : private Service {
  public:
    RustyService() = default;
    virtual ~RustyService() = default;

    RustyService(Service service) : _internal(new Service(service)) {}

    rust::String uuid() const { return rust::String(_internal->uuid()); }

    rust::Vec<SimpleBLE::RustyCharacteristicWrapper> characteristics() const;

  private:
    // NOTE: All internal properties need to be handled as pointers,
    // allowing the calls to RustyService to always be const.
    // This might require us to store pointers to pointers, so it's
    // important to be careful when handling these.
    std::shared_ptr<Service> _internal;
};

class RustyCharacteristic : private Characteristic {
  public:
    RustyCharacteristic() = default;
    virtual ~RustyCharacteristic() = default;

    RustyCharacteristic(Characteristic characteristic) : _internal(new Characteristic(characteristic)) {}

    rust::String uuid() const { return rust::String(_internal->uuid()); }

    rust::Vec<SimpleBLE::RustyDescriptorWrapper> descriptors() const;

  private:
    // NOTE: All internal properties need to be handled as pointers,
    // allowing the calls to RustyCharacteristic to always be const.
    // This might require us to store pointers to pointers, so it's
    // important to be careful when handling these.
    std::shared_ptr<Characteristic> _internal;
};

class RustyDescriptor : private Descriptor {
  public:
    RustyDescriptor() = default;
    virtual ~RustyDescriptor() = default;

    RustyDescriptor(Descriptor descriptor) : _internal(new Descriptor(descriptor)) {}

    rust::String uuid() const { return rust::String(_internal->uuid()); }

  private:
    // NOTE: All internal properties need to be handled as pointers,
    // allowing the calls to RustyDescriptor to always be const.
    // This might require us to store pointers to pointers, so it's
    // important to be careful when handling these.
    std::shared_ptr<Descriptor> _internal;
};

};  // namespace SimpleBLE

rust::Vec<SimpleBLE::RustyAdapterWrapper> RustyAdapter_get_adapters();
bool RustyAdapter_bluetooth_enabled();
