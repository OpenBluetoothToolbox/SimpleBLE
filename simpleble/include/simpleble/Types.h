#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "ByteArray.h"

namespace SimpleBLE {

using BluetoothAddress = std::string;

// IDEA: Extend BluetoothUUID to include a `uuid` function that
// returns the same string, but provides a homogeneous interface.
using BluetoothUUID = std::string;

enum class OperatingSystem {
    WINDOWS,
    MACOS,
    LINUX,
};

// TODO: Add to_string functions for all enums.
enum BluetoothAddressType : int32_t { PUBLIC = 0, RANDOM = 1, UNSPECIFIED = 2 };

}  // namespace SimpleBLE
