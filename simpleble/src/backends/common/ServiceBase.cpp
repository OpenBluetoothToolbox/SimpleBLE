#include <simpleble/Service.h>

#include "ServiceBase.h"
#include "ServiceBuilder.h"

using namespace SimpleBLE;

ServiceBase::ServiceBase(const BluetoothUUID& uuid) : uuid_(uuid) {}

ServiceBase::ServiceBase(const BluetoothUUID& uuid, const ByteArray& data) : uuid_(uuid), data_(data) {}

ServiceBase::ServiceBase(const BluetoothUUID& uuid, std::vector<Characteristic>& characteristics)
    : uuid_(uuid), characteristics_(characteristics) {}

BluetoothUUID ServiceBase::uuid() const { return uuid_; }

ByteArray ServiceBase::data() const { return data_; }

std::vector<Characteristic> ServiceBase::characteristics() const { return characteristics_; }
