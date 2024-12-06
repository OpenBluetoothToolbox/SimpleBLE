
#include "ServiceBase.h"
#include "CommonUtils.h"

using namespace SimpleBLE;

ServiceBase::ServiceBase(const BluetoothUUID& uuid) : uuid_(uuid) {}

ServiceBase::ServiceBase(const BluetoothUUID& uuid, const ByteArray& data) : uuid_(uuid), data_(data) {}

ServiceBase::ServiceBase(const BluetoothUUID& uuid, vec_of_shared<CharacteristicBase>& characteristics)
    : uuid_(uuid), characteristics_(characteristics) {}

BluetoothUUID ServiceBase::uuid() { return uuid_; }

ByteArray ServiceBase::data() { return data_; }

vec_of_shared<CharacteristicBase> ServiceBase::characteristics() { return characteristics_; }
