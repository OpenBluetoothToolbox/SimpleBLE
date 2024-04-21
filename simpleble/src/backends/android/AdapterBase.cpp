#include "AdapterBase.h"
#include "CommonUtils.h"
#include "PeripheralBase.h"
#include "PeripheralBuilder.h"
#include "JniHelper.h"

#include <jni.h>
#include <android/log.h>
// Plenty of useful material here: https://android.googlesource.com/platform/libnativehelper/

using namespace SimpleBLE;

// GlobalRef g_bluetoothManagerClass;
// GlobalRef g_bluetoothAdapterClass;
// GlobalRef g_bluetoothAdapterObject;

JNI::Class bluetoothManagerClass;
JNI::Class bluetoothAdapterClass;
JNI::Object bluetoothAdapterObject;


std::vector<std::shared_ptr<AdapterBase>> AdapterBase::get_adapters() {
    JNI::Env env;

    if (bluetoothAdapterClass.get() == nullptr) {
        bluetoothAdapterClass = env.find_class("android/bluetooth/BluetoothAdapter");
    }

    if (bluetoothAdapterObject.get() == nullptr) {
        bluetoothAdapterObject = bluetoothAdapterClass.call_static_method( "getDefaultAdapter", "()Landroid/bluetooth/BluetoothAdapter;");
    }



    // Create an instance of AdapterBase and add it to the vector
    // TODO: bluetoothAdapterObject must be passed to the constructor of the AdapterBase class
    std::shared_ptr<AdapterBase> adapter = std::make_shared<AdapterBase>();
    std::vector<std::shared_ptr<AdapterBase>> adapters;
    adapters.push_back(adapter);

    return adapters;
}

bool AdapterBase::bluetooth_enabled() {
    int bluetoothState = bluetoothAdapterObject.call_int_method("getState", "()I");
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", fmt::format("Bluetooth state: {}", bluetoothState).c_str());

    return bluetoothAdapterObject.call_boolean_method("isEnabled", "()Z");
}

AdapterBase::AdapterBase() {}

AdapterBase::~AdapterBase() {}

void* AdapterBase::underlying() const { return nullptr; }

std::string AdapterBase::identifier() {
    return bluetoothAdapterObject.call_string_method("getName", "()Ljava/lang/String;");
}

BluetoothAddress AdapterBase::address() {
//    int bluetoothState = bluetoothAdapterObject.call_int_method("getState", "()I");
//    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", fmt::format("Bluetooth state: {}", bluetoothState).c_str());

    std::string address = bluetoothAdapterObject.call_string_method("getAddress", "()Ljava/lang/String;");
    return BluetoothAddress(address);

}

void AdapterBase::scan_start() {}

void AdapterBase::scan_stop() {}

void AdapterBase::scan_for(int timeout_ms) {}

bool AdapterBase::scan_is_active() { return false; }

std::vector<Peripheral> AdapterBase::scan_get_results() {
    return std::vector<Peripheral>();
}

std::vector<Peripheral> AdapterBase::get_paired_peripherals() {
    return std::vector<Peripheral>();
}

void AdapterBase::set_callback_on_scan_start(std::function<void()> on_scan_start) {}

void AdapterBase::set_callback_on_scan_stop(std::function<void()> on_scan_stop) {}

void AdapterBase::set_callback_on_scan_updated(std::function<void(Peripheral)> on_scan_updated) {}

void AdapterBase::set_callback_on_scan_found(std::function<void(Peripheral)> on_scan_found) {}


