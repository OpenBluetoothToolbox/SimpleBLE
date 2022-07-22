#pragma once

#include <memory>

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class ServiceBase;

class Service {
  public:
    Service() = default;
    virtual ~Service() = default;

    BluetoothUUID uuid();

  protected:
    std::shared_ptr<ServiceBase> internal_;
};

}  // namespace SimpleBLE
