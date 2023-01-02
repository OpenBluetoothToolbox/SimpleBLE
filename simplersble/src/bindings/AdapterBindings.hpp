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

    RustyAdapter(Adapter adapter) : _adapter(new Adapter(adapter)){};

    void link(SimpleRsBLE::Adapter const &target) const {}
    void unlink() const {}

    rust::String identifier() const;
    rust::String address() const;

    void scan_start() const;
    void scan_stop() const;
    void scan_for(int32_t timeout_ms) const;
    bool scan_is_active() const;
    rust::Vec<SimpleBLE::RustyPeripheralWrapper> scan_get_results() const;

    void set_callback_on_scan_start(rust::Fn<void()> cb) const {
        std::cout << "Calling callback!\n";
        cb();
        std::cout << "Callback called\n";
    }

    void set_callback_on_scan_start(std::function<void()> on_scan_start) {}
    void set_callback_on_scan_stop(std::function<void()> on_scan_stop) {}

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
