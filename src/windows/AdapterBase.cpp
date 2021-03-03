// This weird pragma is required for the compiler to properly include the necessary namespaces.
#pragma comment(lib, "windowsapp")
#include "AdapterBase.h"

#include "winrt/Windows.Devices.Radios.h"
#include "winrt/Windows.Devices.Bluetooth.h"
#include "winrt/Windows.Devices.Enumeration.h"
#include "winrt/Windows.Foundation.h"
#include "winrt/base.h"

#include <iostream>
#include <sstream>

#define MAC_ADDRESS_STR_LENGTH (size_t) 17 // Two chars per byte, 5 chars for colon

using namespace SimpleBLE;

std::vector<std::shared_ptr<AdapterBase>> AdapterBase::get_adapters() {
    // Initialize the WinRT backend.
    winrt::init_apartment();

    auto device_selector = BluetoothAdapter::GetDeviceSelector();
    auto device_information_collection = Devices::Enumeration::DeviceInformation::FindAllAsync(device_selector).get();

    std::vector<std::shared_ptr<AdapterBase>> adapter_list;
    for (auto& dev_info : device_information_collection) {
        adapter_list.push_back(std::make_shared<AdapterBase>(winrt::to_string(dev_info.Id())));
    }
    return adapter_list;
}

AdapterBase::AdapterBase(std::string device_id)
    : adapter_(BluetoothAdapter::FromIdAsync(winrt::to_hstring(device_id)).get()) {
    
    auto device_information = Devices::Enumeration::DeviceInformation::CreateFromIdAsync(winrt::to_hstring(device_id)).get();
    identifier_ = winrt::to_string(device_information.Name());
}

AdapterBase::~AdapterBase() {}

std::string AdapterBase::identifier() { return identifier_; }

BluetoothAddress AdapterBase::address() { return AdapterBase::_mac_address_to_str(adapter_.BluetoothAddress()); }

std::string AdapterBase::_mac_address_to_str(uint64_t mac_address) {
    uint8_t* mac_address_ptr = (uint8_t*) &mac_address;
    char mac_address_str[MAC_ADDRESS_STR_LENGTH + 1] = {0}; // Include null terminator.

    int position = 0;
    position += sprintf(&mac_address_str[position], "%02x:%02x:%02x:", mac_address_ptr[5], mac_address_ptr[4], mac_address_ptr[3]);
    position += sprintf(&mac_address_str[position], "%02x:%02x:%02x", mac_address_ptr[2], mac_address_ptr[1], mac_address_ptr[0]);
    return std::string((const char*) mac_address_str);
}

uint64_t AdapterBase::_str_to_mac_address(std::string mac_address) {
    // TODO: Validate input - Expected Format: XX:XX:XX:XX:XX:XX
    uint64_t mac_address_number = 0;
    uint8_t* mac_address_ptr = (uint8_t*) &mac_address_number;
    sscanf(&mac_address.c_str()[0], "%02hhx:%02hhx:%02hhx:", &mac_address_ptr[5], &mac_address_ptr[4], &mac_address_ptr[3]);
    sscanf(&mac_address.c_str()[9], "%02hhx:%02hhx:%02hhx:", &mac_address_ptr[2], &mac_address_ptr[1], &mac_address_ptr[0]);
    return mac_address_number;
}