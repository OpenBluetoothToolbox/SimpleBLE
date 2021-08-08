#pragma once

#include <memory>

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>
#include "AdapterBaseTypes.h"

namespace SimpleBLE {

class PeripheralBase {
  public:
    PeripheralBase(void* opaque_peripheral, advertising_data_t advertising_data);
    ~PeripheralBase();

    std::string identifier();
    BluetoothAddress address();

    void connect();
    void disconnect();

  protected:
    /**
     * Holds a pointer to the Objective-C representation of this object.
     */
    void* opaque_internal_;
};

}  // namespace SimpleBLE
