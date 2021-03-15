#pragma once

#include <memory>

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class PeripheralBase;

class Peripheral {
  public:
    Peripheral();
    ~Peripheral();

    std::string identifier();
    BluetoothAddress address();

  protected:
    std::shared_ptr<PeripheralBase> internal_;
    
};

}  // namespace SimpleBLE