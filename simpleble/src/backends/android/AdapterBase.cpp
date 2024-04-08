#include "AdapterBase.h"
#include "CommonUtils.h"
#include "PeripheralBase.h"
#include "PeripheralBuilder.h"

using namespace SimpleBLE;

std::vector<std::shared_ptr<AdapterBase>> AdapterBase::get_adapters() {
    return std::vector<std::shared_ptr<AdapterBase>>();
}

bool AdapterBase::bluetooth_enabled() {
    return true;
}

AdapterBase::AdapterBase() {}

AdapterBase::~AdapterBase() {}

void* AdapterBase::underlying() const { return nullptr; }

std::string AdapterBase::identifier() { return ""; }

BluetoothAddress AdapterBase::address() { return BluetoothAddress(); }

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


#include <jni.h>
#include <android/log.h>
#include <iostream>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    // _jvm = vm;
    // log(DebugLevel::INFO, "OnLoad");

    int value = 42;
    __android_log_print(ANDROID_LOG_INFO, "SimpleBLE", "The answer is: %d", value);

    std::cout << "JNI_OnLoad: Hello Potato!" << std::endl;
    return JNI_VERSION_1_6;
}