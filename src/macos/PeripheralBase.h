#pragma once

#include <memory>

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class PeripheralBase {
  public:
    PeripheralBase();
    ~PeripheralBase();

    std::string identifier();
    BluetoothAddress address();

  private:
};

}  // namespace SimpleBLE