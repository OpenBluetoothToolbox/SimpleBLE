#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "external/kvn_bytearray.h"

/**
 * @file Types.h
 * @brief Defines types and enumerations used throughout the SimpleBLE library.
 */

namespace SimpleBLE {

using BluetoothAddress = std::string;

// IDEA: Extend BluetoothUUID to include a `uuid` function that
// returns the same string, but provides a homogeneous interface.
using BluetoothUUID = std::string;

/**
 * @typedef ByteArray
 * @brief Represents a byte array using kvn::bytearray from the external library.
 */
using ByteArray = kvn::bytearray;

enum class OperatingSystem {
    WINDOWS,
    MACOS,
    LINUX,
};

// TODO: Add to_string functions for all enums.
enum BluetoothAddressType : int32_t { PUBLIC = 0, RANDOM = 1, UNSPECIFIED = 2 };

/**
 * @brief Bluetooth adapter state.
 */
enum PowerState : int32_t {
    /** Power state is unknown. */
    UNKNOWN = 0,
    /** Bluetooth adapter is powered off or unavailable. */
    POWERED_OFF = 1,
    /** Bluetooth adapter is powered on and available. */
    POWERED_ON = 2,
};

}  // namespace SimpleBLE
