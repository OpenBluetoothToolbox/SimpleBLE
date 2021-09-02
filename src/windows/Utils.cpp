#include "Utils.h"

#define MAC_ADDRESS_STR_LENGTH (size_t)17  // Two chars per byte, 5 chars for colon

namespace SimpleBLE {

std::string _mac_address_to_str(uint64_t mac_address) {
    uint8_t* mac_address_ptr = (uint8_t*)&mac_address;
    char mac_address_str[MAC_ADDRESS_STR_LENGTH + 1] = {0};  // Include null terminator.

    int position = 0;
    position += sprintf(&mac_address_str[position], "%02x:%02x:%02x:", mac_address_ptr[5], mac_address_ptr[4],
                        mac_address_ptr[3]);
    position += sprintf(&mac_address_str[position], "%02x:%02x:%02x", mac_address_ptr[2], mac_address_ptr[1],
                        mac_address_ptr[0]);
    return std::string((const char*)mac_address_str);
}

uint64_t _str_to_mac_address(std::string mac_address) {
    // TODO: Validate input - Expected Format: XX:XX:XX:XX:XX:XX
    uint64_t mac_address_number = 0;
    uint8_t* mac_address_ptr = (uint8_t*)&mac_address_number;
    sscanf(&mac_address.c_str()[0], "%02hhx:%02hhx:%02hhx:", &mac_address_ptr[5], &mac_address_ptr[4],
           &mac_address_ptr[3]);
    sscanf(&mac_address.c_str()[9], "%02hhx:%02hhx:%02hhx:", &mac_address_ptr[2], &mac_address_ptr[1],
           &mac_address_ptr[0]);
    return mac_address_number;
}

}  // namespace SimpleBLE
