#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

#include <bluezdbus/BluezAdapter.h>

namespace SimpleBLE {

class AdapterBase {
  public:
    AdapterBase(std::shared_ptr<BluezAdapter> adapter);
    ~AdapterBase();

    std::string identifier();
    BluetoothAddress address();

    void set_callback_on_scan_start(std::function<void()> on_scan_start);
    void set_callback_on_scan_stop(std::function<void()> on_scan_stop);
    void set_callback_on_scan_seen(std::function<void()> on_scan_seen);    // TODO: HOW TO PASS INTERNAL PERIPHERAL?
    void set_callback_on_scan_found(std::function<void()> on_scan_found);  // TODO: HOW TO PASS INTERNAL PERIPHERAL?

    static std::vector<std::shared_ptr<AdapterBase>> get_adapters();

  private:
    std::weak_ptr<BluezAdapter> adapter_;

    std::function<void()> callback_on_scan_start;
    std::function<void()> callback_on_scan_stop;
    std::function<void()> callback_on_scan_seen;   // TODO: HOW TO PASS INTERNAL PERIPHERAL?
    std::function<void()> callback_on_scan_found;  // TODO: HOW TO PASS INTERNAL PERIPHERAL?
};

}  // namespace SimpleBLE