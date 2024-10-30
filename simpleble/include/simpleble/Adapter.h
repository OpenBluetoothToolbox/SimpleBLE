#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <simpleble/export.h>

#include <simpleble/Exceptions.h>
#include <simpleble/Peripheral.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class AdapterBase;

/**
 * Bluetooth Adapter.
 *
 * A default-constructed Adapter object is not initialized. Calling any method
 * other than `initialized()` on an uninitialized Adapter will throw an exception
 * of type `SimpleBLE::NotInitialized`.
 *
 * NOTE: This class is intended to be used by the user only. Library developers
 * should use shared pointers `AdapterBase` instead.
 */
class SIMPLEBLE_EXPORT Adapter {
  public:
    virtual ~Adapter() = default;

    bool initialized() const;

    std::string identifier() const;
    BluetoothAddress address();

    void scan_start();
    void scan_stop();
    void scan_for(int timeout_ms);
    bool scan_is_active();
    std::vector<Peripheral> scan_get_results();

    void set_callback_on_scan_start(std::function<void()> on_scan_start);
    void set_callback_on_scan_stop(std::function<void()> on_scan_stop);
    void set_callback_on_scan_updated(std::function<void(Peripheral)> on_scan_updated);
    void set_callback_on_scan_found(std::function<void(Peripheral)> on_scan_found);

    std::vector<Peripheral> get_paired_peripherals();

    /**
     * Checks if Bluetooth is enabled.
     *
     * The enabled state may be a global setting for the system/backend, or
     * it may be specific to the adapter.
     */
    bool bluetooth_enabled();

    /**
     * Fetches a list of all available adapters from all available backends.
     *
     * This will cause backends to be instantiated/initialized and adapters
     * too.
     */
    static std::vector<Adapter> get_adapters();

  protected:
    AdapterBase* operator->();
    const AdapterBase* operator->() const;

    std::shared_ptr<AdapterBase> internal_;
};

}  // namespace SimpleBLE
