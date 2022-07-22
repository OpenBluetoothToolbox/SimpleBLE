#pragma once

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class ServiceBase {
  public:
    ServiceBase(const BluetoothUUID &uuid);
    virtual ~ServiceBase() = default;

    BluetoothUUID uuid();

  protected:
    BluetoothUUID uuid_;
};

}  // namespace SimpleBLE
