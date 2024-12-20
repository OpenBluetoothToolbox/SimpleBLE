#include "BackendAndroid.h"
#include "AdapterAndroid.h"
#include "BuildVec.h"
#include "CommonUtils.h"

#include <android/log.h>
#include <fmt/core.h>

#include <string>

namespace SimpleBLE {

std::shared_ptr<BackendAndroid> BACKEND_ANDROID() { return BackendAndroid::get(); }

BackendAndroid::BackendAndroid(buildToken) {
    JNI::Env env;

    // Check if the BluetoothAdapter class has been loaded
    if (_btAdapterCls.get() == nullptr) {
        _btAdapterCls = env.find_class("android/bluetooth/BluetoothAdapter");
    }

    if (_btScanResultCls.get() == nullptr) {
        _btScanResultCls = env.find_class("android/bluetooth/le/ScanResult");
    }

    if (_btAdapter.get() == nullptr) {
        _btAdapter = _btAdapterCls.call_static_method("getDefaultAdapter", "()Landroid/bluetooth/BluetoothAdapter;");
    }

    if (_btScanner.get() == nullptr) {
        _btScanner = _btAdapter.call_object_method("getBluetoothLeScanner",
                                                   "()Landroid/bluetooth/le/BluetoothLeScanner;");
    }
}

std::string BackendAndroid::name() const noexcept { return "Android"; }

SharedPtrVector<AdapterBase> BackendAndroid::get_adapters() {
    SharedPtrVector<AdapterBase> adapters;
    adapters.push_back(std::make_shared<AdapterAndroid>(shared_from_this()));

    return adapters;
}

bool BackendAndroid::bluetooth_enabled() {
    bool isEnabled = _btAdapter.call_boolean_method("isEnabled", "()Z");
    int bluetoothState = _btAdapter.call_int_method("getState", "()I");
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", fmt::format("Bluetooth state: {}", bluetoothState).c_str());

    return isEnabled;  // bluetoothState == 12;
}

}  // namespace SimpleBLE
