#pragma once

#include <memory>

#include <simpleble/export.h>

#include <simpleble/Descriptor.h>
#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class CharacteristicBase;

class SIMPLEBLE_EXPORT Characteristic {
  public:
    Characteristic() = default;
    virtual ~Characteristic() = default;

    BluetoothUUID uuid() const;
    std::vector<Descriptor> descriptors() const;
    std::vector<std::string> capabilities() const;

    bool can_read() const;
    bool can_write_request() const;
    bool can_write_command() const;
    bool can_notify() const;
    bool can_indicate() const;

  protected:
    CharacteristicBase &internal();
    const CharacteristicBase &internal() const;
    std::shared_ptr<CharacteristicBase> internal_;
};

}  // namespace SimpleBLE
