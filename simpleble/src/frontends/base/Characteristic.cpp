#include <simpleble/Characteristic.h>

#include "BuildVec.h"
#include "CharacteristicBase.h"

using namespace SimpleBLE;

std::vector<std::string> Characteristic::capabilities() {
    std::vector<std::string> capabilities;

    if (can_read()) {
        capabilities.push_back("read");
    }

    if (can_write_request()) {
        capabilities.push_back("write_request");
    }

    if (can_write_command()) {
        capabilities.push_back("write_command");
    }

    if (can_notify()) {
        capabilities.push_back("notify");
    }

    if (can_indicate()) {
        capabilities.push_back("indicate");
    }

    return capabilities;
}

bool Characteristic::initialized() const { return internal_ != nullptr; }

bool Characteristic::can_read() { return initialized() && internal_->can_read(); }
bool Characteristic::can_write_request() { return initialized() && internal_->can_write_request(); }
bool Characteristic::can_write_command() { return initialized() && internal_->can_write_command(); }
bool Characteristic::can_notify() { return initialized() && internal_->can_notify(); }
bool Characteristic::can_indicate() { return initialized() && internal_->can_indicate(); }

BluetoothUUID Characteristic::uuid() { return initialized() ? internal_->uuid() : BluetoothUUID(); }

std::vector<Descriptor> Characteristic::descriptors() {
    return initialized() ? build_vec(internal_->descriptors()) : std::vector<Descriptor>();
}
