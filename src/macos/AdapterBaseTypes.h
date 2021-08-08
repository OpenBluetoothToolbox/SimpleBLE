#pragma once

#include <simpleble/Types.h>
#include <cstdint>
#include <map>

typedef struct {
    bool connectable;
    int8_t rssi;

    // TODO: Considering that per advertising report only one manufacturer
    // identifier is supported, maybe it is not necessary to use a map.
    std::map<uint16_t, SimpleBLE::ByteArray> manufacturer_data;
} advertising_data_t;
