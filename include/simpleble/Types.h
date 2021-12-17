#pragma once

#include <string>
#include <vector>

namespace SimpleBLE {

typedef std::string BluetoothAddress;

// IDEA: Extend BluetoothUUID to include a `uuid` function that
// returns the same string, but provides a homogeneous interface.
typedef std::string BluetoothUUID;

// IDEA: Extend ByteArray to be constructed by a vector of bytes
// and pointers to uint8_t.
typedef std::string ByteArray;


typedef struct {
    BluetoothUUID uuid;
    std::vector<BluetoothUUID> characteristics;
} BluetoothService;

typedef enum {
    Unknown = 0,
    Resetting = 1,
    Unsupported = 2,
    Unauthorized = 3,
    PoweredOff = 4,
    PoweredOn = 5,
} BluetoothState;

}  // namespace SimpleBLE
