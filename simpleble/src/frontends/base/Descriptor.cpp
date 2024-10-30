#include <simpleble/Descriptor.h>

#include "DescriptorBase.h"

bool SimpleBLE::Descriptor::initialized() const { return internal_ != nullptr; }

SimpleBLE::BluetoothUUID SimpleBLE::Descriptor::uuid() { return initialized() ? internal_->uuid() : BluetoothUUID(); }
