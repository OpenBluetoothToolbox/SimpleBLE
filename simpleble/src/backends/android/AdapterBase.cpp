#include "AdapterBase.h"
#include "CommonUtils.h"
#include "PeripheralBase.h"
#include "PeripheralBuilder.h"

#include <jni.h>
#include <android/log.h>

using namespace SimpleBLE;

JNI::Class AdapterBase::_btAdapterCls;
JNI::Class AdapterBase::_btScanCallbackCls;
JNI::Object AdapterBase::_btAdapter;
JNI::Object AdapterBase::_btScanner;
JNI::Object AdapterBase::_scan_callback;


void AdapterBase::initialize() {
    JNI::Env env;

    // Check if the BluetoothAdapter class has been loaded
    if (_btAdapterCls.get() == nullptr) {
        _btAdapterCls = env.find_class("android/bluetooth/BluetoothAdapter");
    }

    if (_btScanCallbackCls.get() == nullptr) {
        _btScanCallbackCls = env.find_class("org/simpleble/android/bridge/ScanCallback");
        _scan_callback = _btScanCallbackCls.call_constructor("()V");
    }

    //

    if (_btAdapter.get() == nullptr) {
        _btAdapter = _btAdapterCls.call_static_method( "getDefaultAdapter", "()Landroid/bluetooth/BluetoothAdapter;");
    }

    if (_btScanner.get() == nullptr) {
        _btScanner = _btAdapter.call_object_method("getBluetoothLeScanner", "()Landroid/bluetooth/le/BluetoothLeScanner;");
    }


}

std::vector<std::shared_ptr<AdapterBase>> AdapterBase::get_adapters() {
    initialize();

    // Create an instance of AdapterBase and add it to the vector
    std::shared_ptr<AdapterBase> adapter = std::make_shared<AdapterBase>();
    std::vector<std::shared_ptr<AdapterBase>> adapters;
    adapters.push_back(adapter);

    return adapters;
}

bool AdapterBase::bluetooth_enabled() {
    initialize();

    bool isEnabled = _btAdapter.call_boolean_method("isEnabled", "()Z");
    int bluetoothState = _btAdapter.call_int_method("getState", "()I");
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", fmt::format("Bluetooth state: {}", bluetoothState).c_str());

    return isEnabled; //bluetoothState == 12;
}

AdapterBase::AdapterBase() {}

AdapterBase::~AdapterBase() {}

void* AdapterBase::underlying() const { return nullptr; }

std::string AdapterBase::identifier() {
    return _btAdapter.call_string_method("getName", "()Ljava/lang/String;");
}

BluetoothAddress AdapterBase::address() {
    return BluetoothAddress(_btAdapter.call_string_method("getAddress", "()Ljava/lang/String;"));

}

void AdapterBase::scan_start() {
    JNI::Env env;

    _btScanner.call_void_method("startScan", "(Landroid/bluetooth/le/ScanCallback;)V", _scan_callback.get());

}

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


extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_internal_ScanCallback_onScanResultCallback(JNIEnv *env, jobject thiz,
                                                                      jint callback_type,
                                                                      jobject result) {
    // TODO: implement onScanResultCallback()
}
extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_internal_ScanCallback_onScanFailedCallback(JNIEnv *env, jobject thiz,
                                                                      jint error_code) {
    // TODO: implement onScanFailedCallback()
}
extern "C"
JNIEXPORT void JNICALL
Java_org_simpleble_android_internal_ScanCallback_onBatchScanResultsCallback(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jobject results) {
    // TODO: implement onBatchScanResultsCallback()
}