#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <simpleble/Exceptions.h>
#include <simpleble/Peripheral.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class AdapterBase;

class Adapter {
  public:
    Adapter();
    ~Adapter();

    std::string identifier();
    BluetoothAddress address();

    void set_callback_on_scan_start(std::function<void()> on_scan_start);
    void set_callback_on_scan_stop(std::function<void()> on_scan_stop);
    void set_callback_on_scan_seen(std::function<void(Peripheral)> on_scan_seen);
    void set_callback_on_scan_found(std::function<void(Peripheral)> on_scan_found);

    static std::vector<Adapter> get_adapters();

  private:
    std::shared_ptr<AdapterBase> internal_;
};

}  // namespace SimpleBLE