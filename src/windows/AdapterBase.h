#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

#include "winrt/Windows.Devices.Bluetooth.h"

using namespace winrt::Windows;
using namespace winrt::Windows::Devices::Bluetooth;

namespace SimpleBLE {

class AdapterBase {
  public:
    AdapterBase(std::string device_id);
    ~AdapterBase();

    std::string identifier();
    BluetoothAddress address();

    void set_callback_on_scan_start(std::function<void()> on_scan_start);
    void set_callback_on_scan_stop(std::function<void()> on_scan_stop);
    void set_callback_on_scan_seen(std::function<void()> on_scan_seen);    // TODO: HOW TO PASS INTERNAL PERIPHERAL?
    void set_callback_on_scan_found(std::function<void()> on_scan_found);  // TODO: HOW TO PASS INTERNAL PERIPHERAL?

    static std::vector<std::shared_ptr<AdapterBase>> get_adapters();

  private:

    BluetoothAdapter adapter_;
    std::string identifier_;

    std::function<void()> callback_on_scan_start;
    std::function<void()> callback_on_scan_stop;
    std::function<void()> callback_on_scan_seen;   // TODO: HOW TO PASS INTERNAL PERIPHERAL?
    std::function<void()> callback_on_scan_found;  // TODO: HOW TO PASS INTERNAL PERIPHERAL?

    static std::string _mac_address_to_str(uint64_t mac_address);
    static uint64_t _str_to_mac_address(std::string mac_address);
};

}  // namespace SimpleBLE