#pragma once

#include <simpleble/Descriptor.h>
#include <simpleble/Exceptions.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class CharacteristicBase {
  public:
    CharacteristicBase(const BluetoothUUID& uuid, std::vector<Descriptor>& descriptors, bool can_read,
                       bool can_write_request, bool can_write_command, bool can_write_authenticated,
                       bool can_notify, bool can_indicate, bool can_broadcast, bool has_extended_properties);
    virtual ~CharacteristicBase() = default;

    BluetoothUUID uuid();
    std::vector<Descriptor> descriptors();

    bool can_read();
    bool can_write_request();
    bool can_write_command();
    bool can_write_authenticated();
    bool can_notify();
    bool can_indicate();
    bool can_broadcast();
    bool has_extended_properties();

  protected:
    BluetoothUUID uuid_;
    std::vector<Descriptor> descriptors_;

    bool can_read_ = false;
    bool can_write_request_ = false;
    bool can_write_command_ = false;
    bool can_write_authenticated_ = false;
    bool can_notify_ = false;
    bool can_indicate_ = false;
    bool can_broadcast_ = false;
    bool has_extended_properties_ = false;
};

}  // namespace SimpleBLE
