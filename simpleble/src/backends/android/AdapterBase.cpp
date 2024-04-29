#include "AdapterBase.h"
#include "CommonUtils.h"
#include "PeripheralBase.h"
#include "PeripheralBuilder.h"

#include <jni.h>
#include <android/log.h>
#include <thread>
#include <fmt/core.h>

using namespace SimpleBLE;

JNI::Class AdapterBase::_btAdapterCls;
JNI::Class AdapterBase::_btScanCallbackCls;
JNI::Class AdapterBase::_btScanResultCls;

JNI::Object AdapterBase::_btAdapter;
JNI::Object AdapterBase::_btScanner;
std::map<jobject, AdapterBase*, JNI::JObjectComparator> AdapterBase::_scanCallbackMap;


void AdapterBase::initialize() {
    JNI::Env env;

    // Check if the BluetoothAdapter class has been loaded
    if (_btAdapterCls.get() == nullptr) {
        _btAdapterCls = env.find_class("android/bluetooth/BluetoothAdapter");
    }

    if (_btScanCallbackCls.get() == nullptr) {
        _btScanCallbackCls = env.find_class("org/simpleble/android/bridge/ScanCallback");
    }

    if (_btScanResultCls.get() == nullptr) {
        _btScanResultCls = env.find_class("android/bluetooth/le/ScanResult");
    }

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

AdapterBase::AdapterBase() {
    _btScanCallback = _btScanCallbackCls.call_constructor("()V");

    // Add the callback to the map
    _scanCallbackMap[_btScanCallback.get()] = this;
}

AdapterBase::~AdapterBase() {
    // Remove the callback from the map
    _scanCallbackMap.erase(_btScanCallback.get());
}

void* AdapterBase::underlying() const { return nullptr; }

std::string AdapterBase::identifier() {
    return _btAdapter.call_string_method("getName", "()Ljava/lang/String;");
}

BluetoothAddress AdapterBase::address() {
    return BluetoothAddress(_btAdapter.call_string_method("getAddress", "()Ljava/lang/String;"));

}

void AdapterBase::scan_start() {
    JNI::Env env;
    _btScanner.call_void_method("startScan", "(Landroid/bluetooth/le/ScanCallback;)V", _btScanCallback.get());

    auto msg = fmt::format("Scanning started with callback: {}", (void*) _btScanCallback.get());
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg.c_str());
}

void AdapterBase::scan_stop() {
    JNI::Env env;
    _btScanner.call_void_method("stopScan", "(Landroid/bluetooth/le/ScanCallback;)V", _btScanCallback.get());

    auto msg = fmt::format("Scanning stopped with callback: {}", (void*) _btScanCallback.get());
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg.c_str());
}

void AdapterBase::scan_for(int timeout_ms) {
    scan_start();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
    scan_stop();
}

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

void AdapterBase::onScanResultCallback(JNIEnv *env, jobject thiz, jint callback_type, jobject j_scan_result) {
    JNI::Object scanResult(j_scan_result, _btScanResultCls.get());
    std::string str = scanResult.call_string_method("toString", "()Ljava/lang/String;");

    auto msg = fmt::format("onScanResultCallback: {}", str);
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg.c_str());

}

void AdapterBase::onBatchScanResultsCallback(JNIEnv *env, jobject thiz, jobject results) {
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", "onBatchScanResultsCallback");
}

void AdapterBase::onScanFailedCallback(JNIEnv *env, jobject thiz, jint error_code) {
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", "onScanFailedCallback");
}

extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_ScanCallback_onScanResultCallback(JNIEnv *env, jobject thiz, jint callback_type, jobject result) {
    auto it = AdapterBase::_scanCallbackMap.find(thiz);
    if (it != AdapterBase::_scanCallbackMap.end()) {
        AdapterBase* adapter = it->second;
        adapter->onScanResultCallback(env, thiz, callback_type, result);
    } else {
        // TODO: Throw an exception
    }
}
extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_ScanCallback_onScanFailedCallback(JNIEnv *env, jobject thiz, jint error_code) {
    auto it = AdapterBase::_scanCallbackMap.find(thiz);
    if (it != AdapterBase::_scanCallbackMap.end()) {
        AdapterBase* adapter = it->second;
        adapter->onScanFailedCallback(env, thiz, error_code);
    } else {
        // TODO: Throw an exception
    }
}
extern "C" JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_ScanCallback_onBatchScanResultsCallback(JNIEnv *env, jobject thiz, jobject results) {
    auto it = AdapterBase::_scanCallbackMap.find(thiz);
    if (it != AdapterBase::_scanCallbackMap.end()) {
        AdapterBase* adapter = it->second;
        adapter->onBatchScanResultsCallback(env, thiz, results);
    } else {
        // TODO: Throw an exception
    }
}