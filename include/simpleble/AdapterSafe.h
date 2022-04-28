#pragma once

#include <simpleble/Adapter.h>
#include <simpleble/PeripheralSafe.h>

namespace SimpleBLE {

namespace Safe {

class Adapter : public SimpleBLE::Adapter {
  public:
    Adapter(SimpleBLE::Adapter& adapter);
    virtual ~Adapter() = default;

    std::optional<std::string> identifier() const noexcept;
    std::optional<BluetoothAddress> address() const noexcept;

    bool scan_start() noexcept;
    bool scan_stop() noexcept;
    bool scan_for(int timeout_ms) noexcept;
    std::optional<bool> scan_is_active() const noexcept;
    std::optional<std::vector<SimpleBLE::Safe::Peripheral>> scan_get_results() const noexcept;

    bool set_callback_on_scan_start(std::function<void()> on_scan_start) noexcept;
    bool set_callback_on_scan_stop(std::function<void()> on_scan_stop) noexcept;
    bool set_callback_on_scan_updated(std::function<void(SimpleBLE::Safe::Peripheral)> on_scan_updated) noexcept;
    bool set_callback_on_scan_found(std::function<void(SimpleBLE::Safe::Peripheral)> on_scan_found) noexcept;

    std::optional<std::vector<SimpleBLE::Safe::Peripheral>> get_paired_peripherals() const noexcept;

    static std::optional<std::vector<SimpleBLE::Safe::Adapter>> get_adapters() noexcept;
};

}  // namespace Safe

}  // namespace SimpleBLE
