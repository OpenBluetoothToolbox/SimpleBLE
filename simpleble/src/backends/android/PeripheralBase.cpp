#include "PeripheralBase.h"

#include "CharacteristicBuilder.h"
#include "DescriptorBuilder.h"
#include "ServiceBuilder.h"

#include <simpleble/Exceptions.h>
#include <algorithm>
#include "CommonUtils.h"
#include "LoggingInternal.h"

#include <android/log.h>

using namespace SimpleBLE;
using namespace std::chrono_literals;

JNI::Class PeripheralBase::_btGattCallbackCls;

void PeripheralBase::initialize() {
    JNI::Env env;

    if (_btGattCallbackCls.get() == nullptr) {
        _btGattCallbackCls = env.find_class("org/simpleble/android/bridge/BluetoothGattCallback");
    }
}

PeripheralBase::PeripheralBase(Android::ScanResult scan_result) : _device(scan_result.getDevice()) {
    _btGattCallback = _btGattCallbackCls.call_constructor("()V");
}

PeripheralBase::~PeripheralBase() {}

void PeripheralBase::update_advertising_data(Android::ScanResult scan_result) {}

void* PeripheralBase::underlying() const { return nullptr; }

std::string PeripheralBase::identifier() { return _device.getName(); }

BluetoothAddress PeripheralBase::address() { return BluetoothAddress(_device.getAddress()); }

BluetoothAddressType PeripheralBase::address_type() { return BluetoothAddressType::UNSPECIFIED; }

int16_t PeripheralBase::rssi() { return 0; }

int16_t PeripheralBase::tx_power() { return 0; }

uint16_t PeripheralBase::mtu() { return 0; }

void PeripheralBase::connect() {
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", "Connecting to device");
    _device.connectGatt(false, _btGattCallback);
}

void PeripheralBase::disconnect() {}

bool PeripheralBase::is_connected() { return false; }

bool PeripheralBase::is_connectable() { return false; }

bool PeripheralBase::is_paired() { return false; }

void PeripheralBase::unpair() {}

std::vector<Service> PeripheralBase::services() { return std::vector<Service>(); }

std::vector<Service> PeripheralBase::advertised_services() { return std::vector<Service>(); }

std::map<uint16_t, ByteArray> PeripheralBase::manufacturer_data() { return std::map<uint16_t, ByteArray>(); }

ByteArray PeripheralBase::read(BluetoothUUID const& service, BluetoothUUID const& characteristic) {
    return ByteArray();
}

void PeripheralBase::write_request(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                                   ByteArray const& data) {}

void PeripheralBase::write_command(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                                   ByteArray const& data) {}

void PeripheralBase::notify(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                            std::function<void(ByteArray payload)> callback) {}

void PeripheralBase::indicate(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                              std::function<void(ByteArray payload)> callback) {}

void PeripheralBase::unsubscribe(BluetoothUUID const& service, BluetoothUUID const& characteristic) {}

ByteArray PeripheralBase::read(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                               BluetoothUUID const& descriptor) {
    return ByteArray();
}

void PeripheralBase::write(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                           BluetoothUUID const& descriptor, ByteArray const& data) {}

void PeripheralBase::set_callback_on_connected(std::function<void()> on_connected) {}

void PeripheralBase::set_callback_on_disconnected(std::function<void()> on_disconnected) {}

extern "C" {

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onCharacteristicChangedCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject characteristic, jbyteArray value) {
    // TODO: implement onCharacteristicChangedCallback()
    auto msg = "onCharacteristicChangedCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onCharacteristicReadCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject characteristic, jbyteArray value, jint status) {
    // TODO: implement onCharacteristicReadCallback()
    auto msg = "onCharacteristicReadCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onCharacteristicWriteCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject characteristic, jint status) {
    // TODO: implement onCharacteristicWriteCallback()
    auto msg = "onCharacteristicWriteCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onConnectionStateChangeCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint status, jint new_state) {
    // TODO: implement onConnectionStateChangeCallback()
    auto msg = "onConnectionStateChangeCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onDescriptorReadCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject descriptor, jbyteArray value, jint status) {
    // TODO: implement onDescriptorReadCallback()
    auto msg = "onDescriptorReadCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onDescriptorWriteCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject descriptor, jint status) {
    // TODO: implement onDescriptorWriteCallback()
    auto msg = "onDescriptorWriteCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onMtuChangedCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint mtu, jint status) {
    // TODO: implement onMtuChangedCallback()
    auto msg = "onMtuChangedCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onPhyReadCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint tx_phy, jint rx_phy, jint status) {
    // TODO: implement onPhyReadCallback()
    auto msg = "onPhyReadCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onPhyUpdateCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint tx_phy, jint rx_phy, jint status) {
    // TODO: implement onPhyUpdateCallback()
    auto msg = "onPhyUpdateCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onReadRemoteRssiCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint rssi, jint status) {
    // TODO: implement onReadRemoteRssiCallback()
    auto msg = "onReadRemoteRssiCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onReliableWriteCompletedCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint status) {
    // TODO: implement onReliableWriteCompletedCallback()
    auto msg = "onReliableWriteCompletedCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onServiceChangedCallback(
    JNIEnv* env, jobject thiz, jobject gatt) {
    // TODO: implement onServiceChangedCallback()
    auto msg = "onServiceChangedCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onServicesDiscoveredCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint status) {
    // TODO: implement onServicesDiscoveredCallback()
    auto msg = "onServicesDiscoveredCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

}  // extern "C"