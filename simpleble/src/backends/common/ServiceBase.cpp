
#include "ServiceBase.h"
#include "BuilderBase.h"
#include "simpleble/Characteristic.h"

using namespace SimpleBLE;

ServiceBase::ServiceBase(const BluetoothUUID& uuid) : uuid_(uuid) {}

ServiceBase::ServiceBase(const BluetoothUUID& uuid, const ByteArray& data) : uuid_(uuid), data_(data) {}

ServiceBase::ServiceBase(const BluetoothUUID& uuid, std::vector<std::shared_ptr<CharacteristicBase>>& characteristics)
    : uuid_(uuid) {
    for (auto& characteristic_base : characteristics) {
        characteristics_.push_back(build(characteristic_base));
    }
}

BluetoothUUID ServiceBase::uuid() { return uuid_; }

ByteArray ServiceBase::data() { return data_; }

std::vector<Characteristic> ServiceBase::characteristics() { return characteristics_; }
