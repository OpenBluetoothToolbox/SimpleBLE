#include <simpleble/Descriptor.h>

#include "DescriptorBase.h"

using namespace SimpleBLE;

BluetoothUUID Descriptor::uuid() { return internal_->uuid(); }
