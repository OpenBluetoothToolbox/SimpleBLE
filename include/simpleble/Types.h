#pragma once

#include <string>
#include <vector>

namespace SimpleBLE {

typedef std::string BluetoothAddress;
typedef std::string BluetoothUUID;
typedef std::string ByteArray;

// IDEA: Extend BluetoothUUID to include a `uuid` function that
// returns the same string, but provides a homogeneous interface.

typedef struct {
    BluetoothUUID uuid;
    std::vector<BluetoothUUID> characteristics;
} BluetoothService;

}  // namespace SimpleBLE
