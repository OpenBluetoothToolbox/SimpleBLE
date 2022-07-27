#pragma once

#include <memory>
#include <vector>

#include "simpleble/Characteristic.h"
#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class ServiceBase;

class Service {
  public:
    Service() = default;
    virtual ~Service() = default;

    BluetoothUUID uuid();
    std::vector<Characteristic> characteristics();

  protected:
    std::shared_ptr<ServiceBase> internal_;
};

}  // namespace SimpleBLE
