#include <simpleble/Characteristic.h>

#include "CharacteristicBase.h"
#include "CharacteristicBuilder.h"

using namespace SimpleBLE;

BluetoothUUID Characteristic::uuid() const { return internal().uuid(); }

std::vector<Descriptor> Characteristic::descriptors() const { return internal().descriptors(); }

std::vector<std::string> Characteristic::capabilities() const {
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

bool Characteristic::can_read() const { return internal().can_read(); }
bool Characteristic::can_write_request() const { return internal().can_write_request(); }
bool Characteristic::can_write_command() const { return internal().can_write_command(); }
bool Characteristic::can_notify() const { return internal().can_notify(); }
bool Characteristic::can_indicate() const { return internal().can_indicate(); }

CharacteristicBase &Characteristic::internal() { return *internal_; }
const CharacteristicBase &Characteristic::internal() const { return *internal_; }
