#pragma once

#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class CharacteristicBase {
  public:
    CharacteristicBase(const BluetoothUUID &uuid);
    virtual ~CharacteristicBase() = default;

    BluetoothUUID uuid();

  protected:
    BluetoothUUID uuid_;
};

}  // namespace SimpleBLE
