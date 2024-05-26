#include <simpleble/Service.h>

#include "ServiceBase.h"
#include "ServiceBuilder.h"

using namespace SimpleBLE;

BluetoothUUID Service::uuid() const { return internal().uuid(); }

ByteArray Service::data() const { return internal().data(); }

std::vector<Characteristic> Service::characteristics() const { return internal().characteristics(); }

ServiceBase &Service::internal() { return *internal_; }

const ServiceBase &Service::internal() const { return *internal_; }
