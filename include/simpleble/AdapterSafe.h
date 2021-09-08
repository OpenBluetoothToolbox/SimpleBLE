#pragma once

#include <simpleble/Adapter.h>
#include <simpleble/PeripheralSafe.h>

namespace SimpleBLE {

class AdapterSafe : public Adapter {
  public:
    AdapterSafe(Adapter& adapter);
    ~AdapterSafe();

    std::optional<std::string> identifier() noexcept;
    std::optional<BluetoothAddress> address() noexcept;

    bool scan_start() noexcept;
    bool scan_stop() noexcept;
    bool scan_for(int timeout_ms) noexcept;
    std::optional<bool> scan_is_active() noexcept;
    std::optional<std::vector<PeripheralSafe>> scan_get_results() noexcept;

    bool set_callback_on_scan_start(std::function<void()> on_scan_start) noexcept;
    bool set_callback_on_scan_stop(std::function<void()> on_scan_stop) noexcept;
    bool set_callback_on_scan_updated(std::function<void(PeripheralSafe)> on_scan_updated) noexcept;
    bool set_callback_on_scan_found(std::function<void(PeripheralSafe)> on_scan_found) noexcept;

    static std::optional<std::vector<AdapterSafe>> get_adapters() noexcept;
};

}  // namespace SimpleBLE
