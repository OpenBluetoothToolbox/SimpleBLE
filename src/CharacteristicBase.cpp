#include <simpleble/Characteristic.h>

#include "CharacteristicBase.h"
#include "CharacteristicBuilder.h"

using namespace SimpleBLE;

CharacteristicBase::CharacteristicBase(const BluetoothUUID& uuid) : uuid_(uuid) {}

BluetoothUUID CharacteristicBase::uuid() { return uuid_; }