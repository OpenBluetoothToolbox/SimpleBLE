#include <CommonUtils.h>
#include "BluetoothGattCallback.h"

#include <android/log.h>

namespace SimpleBLE {
namespace Android {
namespace Bridge {

JNI::Class BluetoothGattCallback::_cls;
std::map<jobject, BluetoothGattCallback*, JNI::JObjectComparator> BluetoothGattCallback::_map;

void BluetoothGattCallback::initialize() {
    JNI::Env env;

    if (_cls.get() == nullptr) {
        _cls = env.find_class("org/simpleble/android/bridge/BluetoothGattCallback");
    }
}

BluetoothGattCallback::BluetoothGattCallback() {
    initialize();

    _obj = _cls.call_constructor("()V");
    _map[_obj.get()] = this;
}

BluetoothGattCallback::~BluetoothGattCallback() {
    _map.erase(_obj.get());
}

void BluetoothGattCallback::set_callback_onConnectionStateChange(std::function<void(bool)> callback) {
    if (callback) {
        _callback_onConnectionStateChange.load(callback);
    } else {
        _callback_onConnectionStateChange.unload();
    }
}


void BluetoothGattCallback::jni_onCharacteristicChangedCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject characteristic, jbyteArray value) {
    auto msg = "onCharacteristicChangedCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onCharacteristicReadCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject characteristic, jbyteArray value, jint status) {
    auto msg = "onCharacteristicReadCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onCharacteristicWriteCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject characteristic, jint status) {
    auto msg = "onCharacteristicWriteCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onConnectionStateChangeCallback(JNIEnv *env, jobject thiz, jobject gatt, jint status, jint new_state) {
    auto msg = "onConnectionStateChangeCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);

    auto it = BluetoothGattCallback::_map.find(thiz);
    if (it != BluetoothGattCallback::_map.end()) {
        BluetoothGattCallback* obj = it->second;

        auto msg = "Found object!!";
        __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);

        SAFE_CALLBACK_CALL(obj->_callback_onConnectionStateChange, new_state == 2);
    } else {
        // TODO: Throw an exception
    }
}

void BluetoothGattCallback::jni_onDescriptorReadCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject descriptor, jbyteArray value, jint status) {
    auto msg = "onDescriptorReadCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onDescriptorWriteCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject descriptor, jint status) {
    auto msg = "onDescriptorWriteCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onMtuChangedCallback(JNIEnv *env, jobject thiz, jobject gatt, jint mtu, jint status) {
    auto msg = "onMtuChangedCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onPhyReadCallback(JNIEnv *env, jobject thiz, jobject gatt, jint tx_phy, jint rx_phy, jint status) {
    auto msg = "onPhyReadCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onPhyUpdateCallback(JNIEnv *env, jobject thiz, jobject gatt, jint tx_phy, jint rx_phy, jint status) {
    auto msg = "onPhyUpdateCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onReadRemoteRssiCallback(JNIEnv *env, jobject thiz, jobject gatt, jint rssi, jint status) {
    auto msg = "onReadRemoteRssiCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onReliableWriteCompletedCallback(JNIEnv *env, jobject thiz, jobject gatt, jint status) {
    auto msg = "onReliableWriteCompletedCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onServiceChangedCallback(JNIEnv *env, jobject thiz, jobject gatt) {
    auto msg = "onServiceChangedCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

void BluetoothGattCallback::jni_onServicesDiscoveredCallback(JNIEnv *env, jobject thiz, jobject gatt, jint status) {
    auto msg = "onServicesDiscoveredCallback";
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg);
}

}  // namespace Bridge
}  // namespace Android
}  // namespace SimpleBLE


extern "C" {
// clang-format off
JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onCharacteristicChangedCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject characteristic, jbyteArray value) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onCharacteristicChangedCallback(env, thiz, gatt, characteristic, value);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onCharacteristicReadCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject characteristic, jbyteArray value, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onCharacteristicReadCallback(env, thiz, gatt, characteristic, value, status);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onCharacteristicWriteCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject characteristic, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onCharacteristicWriteCallback(env, thiz, gatt, characteristic, status);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onConnectionStateChangeCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint status, jint new_state) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onConnectionStateChangeCallback(env, thiz, gatt, status, new_state);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onDescriptorReadCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject descriptor, jbyteArray value, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onDescriptorReadCallback(env, thiz, gatt, descriptor, value, status);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onDescriptorWriteCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jobject descriptor, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onDescriptorWriteCallback(env, thiz, gatt, descriptor, status);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onMtuChangedCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint mtu, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onMtuChangedCallback(env, thiz, gatt, mtu, status);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onPhyReadCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint tx_phy, jint rx_phy, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onPhyReadCallback(env, thiz, gatt, tx_phy, rx_phy, status);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onPhyUpdateCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint tx_phy, jint rx_phy, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onPhyUpdateCallback(env, thiz, gatt, tx_phy, rx_phy, status);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onReadRemoteRssiCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint rssi, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onReadRemoteRssiCallback(env, thiz, gatt, rssi, status);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onReliableWriteCompletedCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onReliableWriteCompletedCallback(env, thiz, gatt, status);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onServiceChangedCallback(
    JNIEnv* env, jobject thiz, jobject gatt) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onServiceChangedCallback(env, thiz, gatt);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_BluetoothGattCallback_onServicesDiscoveredCallback(
    JNIEnv* env, jobject thiz, jobject gatt, jint status) {
    SimpleBLE::Android::Bridge::BluetoothGattCallback::jni_onServicesDiscoveredCallback(env, thiz, gatt, status);
}
// clang-format on
}  // extern "C"