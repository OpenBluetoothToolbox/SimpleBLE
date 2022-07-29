#pragma once

#include <simpleble/Adapter.h>
#include <simpleble/PeripheralSafe.h>

namespace SimpleBLE {

namespace Safe {

class Adapter : public SimpleBLE::Adapter {
  public:
    Adapter(SimpleBLE::Adapter& adapter);
    virtual ~Adapter() = default;

    std::optional<std::string> identifier() noexcept;
    std::optional<BluetoothAddress> address() noexcept;

    bool scan_start() noexcept;
    bool scan_start_with_services(std::vector<BluetoothUUID> service_uuids) noexcept;
    bool scan_stop() noexcept;
    bool scan_for(int timeout_ms) noexcept;
    bool scan_for_with_services(int timeout_ms, std::vector<BluetoothUUID> service_uuids) noexcept;
    std::optional<bool> scan_is_active() noexcept;
    std::optional<std::vector<SimpleBLE::Safe::Peripheral>> scan_get_results() noexcept;

    bool set_callback_on_scan_start(std::function<void()> on_scan_start) noexcept;
    bool set_callback_on_scan_stop(std::function<void()> on_scan_stop) noexcept;
    bool set_callback_on_scan_updated(std::function<void(SimpleBLE::Safe::Peripheral)> on_scan_updated) noexcept;
    bool set_callback_on_scan_found(std::function<void(SimpleBLE::Safe::Peripheral)> on_scan_found) noexcept;

    std::optional<std::vector<SimpleBLE::Safe::Peripheral>> get_paired_peripherals() noexcept;

    static std::optional<std::vector<SimpleBLE::Safe::Adapter>> get_adapters() noexcept;
};

}  // namespace Safe

}  // namespace SimpleBLE
