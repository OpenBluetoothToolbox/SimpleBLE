#pragma once

#include "rust/cxx.h"
#include "simpleble/Adapter.h"
#include "simpleble/Peripheral.h"

#include <cstdint>
#include <iostream>
#include <memory>

namespace SimpleRsBLE {

struct Adapter;

};

namespace SimpleBLE {

struct RustyAdapterWrapper;
struct RustyPeripheralWrapper;

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

    RustyPeripheral(Peripheral peripheral) : _peripheral(new Peripheral(peripheral)) {}

    rust::String identifier() const { return rust::String(_peripheral->identifier()); }
    rust::String address() const { return rust::String(_peripheral->address()); }

  private:
    std::shared_ptr<Peripheral> _peripheral;
};

};  // namespace SimpleBLE

rust::Vec<SimpleBLE::RustyAdapterWrapper> RustyAdapter_get_adapters();
bool RustyAdapter_bluetooth_enabled();
