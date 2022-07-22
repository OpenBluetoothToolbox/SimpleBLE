#include <simpleble/Service.h>

#include "ServiceBase.h"
#include "ServiceBuilder.h"

using namespace SimpleBLE;

ServiceBase::ServiceBase(const BluetoothUUID& uuid) : uuid_(uuid) {}

BluetoothUUID ServiceBase::uuid() { return uuid_; }