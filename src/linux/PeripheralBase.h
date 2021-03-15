#pragma once

#include <memory>

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

#include <bluezdbus/BluezDevice.h>

namespace SimpleBLE {

class PeripheralBase {
  public:
    PeripheralBase(std::shared_ptr<BluezDevice> device);
    ~PeripheralBase();

    std::string identifier();
    BluetoothAddress address();

  private:
    std::weak_ptr<BluezDevice> device_;
};

}  // namespace SimpleBLE