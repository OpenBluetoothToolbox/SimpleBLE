#pragma once

#include <string>
#include <vector>

namespace SimpleBLE {

typedef std::string BluetoothAddress;
typedef std::string BluetoothUUID;
typedef std::string ByteArray;

typedef struct {
    BluetoothUUID service_uuid;
    std::vector<BluetoothUUID> characteristic_uuids;
} BluetoothService;

}  // namespace SimpleBLE
