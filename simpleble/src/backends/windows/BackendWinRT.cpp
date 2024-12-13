#include "BackendWindows.h"

#include "AdapterWindows.h"
#include "CommonUtils.h"

#include "winrt/Windows.Devices.Bluetooth.h"
#include "winrt/Windows.Devices.Enumeration.h"
#include "winrt/Windows.Devices.Radios.h"
#include "winrt/Windows.Foundation.Collections.h"

#include "Utils.h"

namespace SimpleBLE {

std::shared_ptr<BackendBase> BACKEND_WINDOWS() { return BackendWindows::get(); }

BackendWindows::BackendWindows(buildToken) { initialize_winrt(); }

bool BackendWindows::bluetooth_enabled() {
    bool enabled = false;
    auto radio_collection = async_get(Radio::GetRadiosAsync());
    for (uint32_t i = 0; i < radio_collection.Size(); i++) {
        auto radio = radio_collection.GetAt(i);

        // Skip non-bluetooth radios
        if (radio.Kind() != RadioKind::Bluetooth) {
            continue;
        }

        // Assume that bluetooth is enabled if any of the radios are enabled
        if (radio.State() == RadioState::On) {
            enabled = true;
            break;
        }
    }

    return enabled;
}

vec_of_shared<AdapterBase> BackendWindows::get_adapters() {
    auto device_selector = BluetoothAdapter::GetDeviceSelector();
    auto device_information_collection = async_get(
        Devices::Enumeration::DeviceInformation::FindAllAsync(device_selector));

    vec_of_shared<AdapterBase> adapter_list;
    for (uint32_t i = 0; i < device_information_collection.Size(); i++) {
        auto dev_info = device_information_collection.GetAt(i);
        adapter_list.push_back(std::make_shared<AdapterWindows>(winrt::to_string(dev_info.Id())));
    }
    return adapter_list;
}

std::string BackendWindows::backend_name() const noexcept { return "Windows"; }

};  // namespace SimpleBLE
