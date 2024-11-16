#pragma once

#include <simpleble/Exceptions.h>
#include <simpleble/Peripheral.h>
#include <simpleble/Types.h>

#include <kvn_safe_callback.hpp>

#include <atomic>
#include <functional>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace SimpleBLE {

class AdapterBase {
  public:
    AdapterBase() = default;
    virtual ~AdapterBase() = default;

    virtual void* underlying() const = 0;

    virtual std::string identifier() = 0;
    virtual BluetoothAddress address() = 0;

    virtual void scan_start() = 0;
    virtual void scan_stop() = 0;
    virtual void scan_for(int timeout_ms) = 0;
    virtual bool scan_is_active() = 0;
    virtual std::vector<Peripheral> scan_get_results() = 0;

    virtual void set_callback_on_scan_start(std::function<void()> on_scan_start) = 0;
    virtual void set_callback_on_scan_stop(std::function<void()> on_scan_stop) = 0;
    virtual void set_callback_on_scan_updated(std::function<void(Peripheral)> on_scan_updated) = 0;
    virtual void set_callback_on_scan_found(std::function<void(Peripheral)> on_scan_found) = 0;

    virtual std::vector<Peripheral> get_paired_peripherals() = 0;

    // static virtual bool bluetooth_enabled();
    // static virtual std::vector<std::shared_ptr<AdapterBase>> get_adapters();
};

}  // namespace SimpleBLE
