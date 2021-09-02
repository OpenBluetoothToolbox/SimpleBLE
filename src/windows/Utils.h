#pragma once

#include <string>
#include <cstdint>

namespace SimpleBLE {

std::string _mac_address_to_str(uint64_t mac_address);
uint64_t _str_to_mac_address(std::string mac_address);

}  // namespace SimpleBLE