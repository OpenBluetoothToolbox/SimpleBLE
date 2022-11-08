#pragma once

#include <simpleble/Types.h>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace SimpleBLE {

struct advertising_data_t {
    std::string identifier;
    BluetoothAddress mac_address;
    bool connectable;
    int16_t rssi;

    std::map<uint16_t, SimpleBLE::ByteArray> manufacturer_data;

    std::vector<std::string> service_uuids;
};

}  // namespace SimpleBLE
