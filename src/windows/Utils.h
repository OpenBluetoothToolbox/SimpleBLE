#pragma once

#include <string>
#include <cstdint>

#include "winrt/base.h"

namespace SimpleBLE {

std::string _mac_address_to_str(uint64_t mac_address);
uint64_t _str_to_mac_address(std::string mac_address);

winrt::guid uuid_to_guid(const std::string& uuid);
std::string guid_to_uuid(const winrt::guid& guid);

}  // namespace SimpleBLE