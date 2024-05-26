#pragma once

#include <simpleble/Descriptor.h>
#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class CharacteristicBase {
  public:
    CharacteristicBase(const BluetoothUUID& uuid, std::vector<Descriptor>& descriptors, bool can_read,
                       bool can_write_request, bool can_write_command, bool can_notify, bool can_indicate);
    virtual ~CharacteristicBase() = default;

    BluetoothUUID uuid() const;
    std::vector<Descriptor> descriptors() const;

    bool can_read() const;
    bool can_write_request() const;
    bool can_write_command() const;
    bool can_notify() const;
    bool can_indicate() const;

  protected:
    BluetoothUUID uuid_;
    std::vector<Descriptor> descriptors_;

    bool can_read_ = false;
    bool can_write_request_ = false;
    bool can_write_command_ = false;
    bool can_notify_ = false;
    bool can_indicate_ = false;
};

}  // namespace SimpleBLE
