#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <simpleble/Exceptions.h>
#include <simpleble/Peripheral.h>
#include <simpleble/Types.h>
#include "AdapterBaseTypes.h"

namespace SimpleBLE {

/**
  This class definition acts as an abstraction layer between C++ and Objective-C.
  If Objective-C headers are included here, everything blows up.
 */
class AdapterBase {
  public:
    AdapterBase();
    virtual ~AdapterBase();

    std::string identifier();
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
    void set_callback_on_state_changed(std::function<void(BluetoothState)> on_state_changed);

    static std::vector<std::shared_ptr<AdapterBase> > get_adapters();

    void delegate_did_discover_peripheral(void* opaque_peripheral, void* opaque_adapter,
                                          advertising_data_t advertising_data);
    void delegate_did_connect_peripheral(void* opaque_peripheral);
    void delegate_did_disconnect_peripheral(void* opaque_peripheral);
    void delegate_did_change_state(BluetoothState state);

  protected:
    /**
     * Holds a pointer to the Objective-C representation of this object.
     */
    void* opaque_internal_;

    std::function<void()> callback_on_scan_start_;
    std::function<void()> callback_on_scan_stop_;
    std::function<void(Peripheral)> callback_on_scan_updated_;
    std::function<void(Peripheral)> callback_on_scan_found_;
    std::function<void(BluetoothState)> callback_on_state_changed_;

    /**
     * Holds a map of objective-c peripheral objects to their corresponding C++ objects.
     *
     * Whenever some sort of event happens on a peripheral, we need to find
     * the corresponding PeripheralBase object. The PeripheralBase objects
     * can be wrapped in disposable Peripheral objects, so there is no need
     * be careful with them.
     */
    std::map<void*, std::shared_ptr<PeripheralBase> > peripherals_;
};

}  // namespace SimpleBLE
