#pragma once

#include <memory>
#include <vector>

#include <simpleble/export.h>

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>
#include "simpleble/Characteristic.h"

namespace SimpleBLE {

class ServiceBase;

class SIMPLEBLE_EXPORT Service {
  public:
    Service() = default;
    virtual ~Service() = default;

    BluetoothUUID uuid() const;
    ByteArray data() const;
    std::vector<Characteristic> characteristics() const;

  protected:
    ServiceBase &internal();
    const ServiceBase &internal() const;
    std::shared_ptr<ServiceBase> internal_;
};

}  // namespace SimpleBLE
