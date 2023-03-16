#include "ServiceBuilder.h"

#include "ServiceBase.h"

using namespace SimpleBLE;

ServiceBuilder::ServiceBuilder(const BluetoothUUID& uuid) { internal_ = std::make_shared<ServiceBase>(uuid); }

ServiceBuilder::ServiceBuilder(const BluetoothUUID& uuid, const ByteArray& data) {
    internal_ = std::make_shared<ServiceBase>(uuid, data);
}

ServiceBuilder::ServiceBuilder(const BluetoothUUID& uuid, std::vector<Characteristic> characteristics, std::vector<BluetoothUUID> included_services) {
    internal_ = std::make_shared<ServiceBase>(uuid, characteristics, included_services);
}
