#include <simpleble/Characteristic.h>

#include "CharacteristicBase.h"
#include "CharacteristicBuilder.h"

using namespace SimpleBLE;

CharacteristicBase::CharacteristicBase(const BluetoothUUID& uuid, std::vector<Descriptor>& descriptors, bool can_read,
                                       bool can_write_request, bool can_write_command, bool can_write_authenticated,
                                       bool can_notify, bool can_indicate, bool can_broadcast, bool has_extended_properties)
    : uuid_(uuid),
      descriptors_(descriptors),
      can_read_(can_read),
      can_write_request_(can_write_request),
      can_write_command_(can_write_command),
      can_write_authenticated_(can_write_authenticated),
      can_notify_(can_notify),
      can_indicate_(can_indicate),
      can_broadcast_(can_broadcast),
      has_extended_properties_(has_extended_properties) {}

BluetoothUUID CharacteristicBase::uuid() { return uuid_; }

std::vector<Descriptor> CharacteristicBase::descriptors() { return descriptors_; }

bool CharacteristicBase::can_read() { return can_read_; }
bool CharacteristicBase::can_write_request() { return can_write_request_; }
bool CharacteristicBase::can_write_command() { return can_write_command_; }
bool CharacteristicBase::can_write_authenticated() { return can_write_authenticated_; }
bool CharacteristicBase::can_notify() { return can_notify_; }
bool CharacteristicBase::can_indicate() { return can_indicate_; }
bool CharacteristicBase::can_broadcast() { return can_broadcast_; }
bool CharacteristicBase::has_extended_properties() { return has_extended_properties_; }
