#pragma once

#include <memory>

#include <simpleble/Descriptor.h>
#include <simpleble/Exceptions.h>
#include <simpleble/Export.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class CharacteristicBase;

class SIMPLEBLE_EXPORT Characteristic {
  public:
    Characteristic() = default;
    virtual ~Characteristic() = default;

    BluetoothUUID uuid();
    std::vector<Descriptor> descriptors();

  protected:
    std::shared_ptr<CharacteristicBase> internal_;
};

}  // namespace SimpleBLE
