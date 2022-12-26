#pragma once

#include "rust/cxx.h"
#include "simpleble/Adapter.h"
#include "simpleble/Peripheral.h"

#include <cstdint>
#include <iostream>
#include <memory>

namespace SimpleBLE {

struct RustyAdapterWrapper;

struct RustyPeripheralWrapper;

class RustyAdapter : private Adapter {
  public:
    RustyAdapter() = default;
    virtual ~RustyAdapter() = default;

    RustyAdapter(Adapter adapter) : _adapter(new Adapter(adapter)){};

    rust::String identifier() const;

    rust::String address() const;

    void scan_start() const;

    void scan_stop() const;

    void scan_for(int32_t timeout_ms) const;

    bool scan_is_active() const;

    rust::Vec<SimpleBLE::RustyPeripheralWrapper> scan_get_results() const;

  private:
    std::shared_ptr<Adapter> _adapter;
};

class RustyPeripheral : private Peripheral {
  public:
    RustyPeripheral() = default;
    virtual ~RustyPeripheral() = default;

    RustyPeripheral(Peripheral peripheral) : _peripheral(new Peripheral(peripheral)) {}

    rust::String identifier() const { return rust::String(_peripheral->identifier()); }

    rust::String address() const { return rust::String(_peripheral->address()); }

  private:
    std::shared_ptr<Peripheral> _peripheral;
};

};  // namespace SimpleBLE

rust::Vec<SimpleBLE::RustyAdapterWrapper> RustyAdapter_get_adapters();
bool RustyAdapter_bluetooth_enabled();
