#include <simpleble/Service.h>

#include "ServiceBase.h"
#include "ServiceBuilder.h"

using namespace SimpleBLE;

ServiceBase::ServiceBase(const BluetoothUUID& uuid) : uuid_(uuid) {}

ServiceBase::ServiceBase(const BluetoothUUID& uuid, const ByteArray& data) : uuid_(uuid), data_(data) {}

ServiceBase::ServiceBase(const BluetoothUUID& uuid, std::vector<Characteristic>& characteristics,
                         std::vector<BluetoothUUID>& included_services)
    : uuid_(uuid), characteristics_(characteristics), included_services_(included_services) {}

BluetoothUUID ServiceBase::uuid() { return uuid_; }

ByteArray ServiceBase::data() { return data_; }

std::vector<Characteristic> ServiceBase::characteristics() { return characteristics_; }

std::vector<BluetoothUUID> ServiceBase::included_services() { return included_services_; }
