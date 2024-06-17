#pragma once

#include "jni/Common.hpp"
#include <kvn_safe_callback.hpp>
#include <map>
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace SimpleBLE {
namespace Android {
namespace Bridge {

class BluetoothGattCallback {
  public:
    BluetoothGattCallback();
    virtual ~BluetoothGattCallback();
    jobject get() { return _obj.get(); } // TODO: Remove once nothing uses this

    void set_callback_onConnectionStateChange(std::function<void(bool)> callback);
    void set_callback_onServicesDiscovered(std::function<void(void)> callback);

    void set_callback_onCharacteristicChanged(JNI::Object characteristic, std::function<void(std::vector<uint8_t> value)> callback);
    void clear_callback_onCharacteristicChanged(JNI::Object characteristic);

    void set_flag_descriptorWritePending(JNI::Object descriptor);
    void clear_flag_descriptorWritePending(JNI::Object descriptor);
    void wait_flag_descriptorWritePending(JNI::Object descriptor);

    bool connected;
    bool services_discovered;
    uint16_t mtu;

    // Not for public use
    // clang-format off
    static void jni_onConnectionStateChangeCallback(JNIEnv *env, jobject thiz, jobject gatt, jint status, jint new_state);
    static void jni_onServicesDiscoveredCallback(JNIEnv *env, jobject thiz, jobject gatt, jint status);
    static void jni_onServiceChangedCallback(JNIEnv *env, jobject thiz, jobject gatt);

    static void jni_onCharacteristicChangedCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject characteristic, jbyteArray value);
    static void jni_onCharacteristicReadCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject characteristic, jbyteArray value, jint status);
    static void jni_onCharacteristicWriteCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject characteristic, jint status);

    static void jni_onDescriptorReadCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject descriptor, jbyteArray value, jint status);
    static void jni_onDescriptorWriteCallback(JNIEnv *env, jobject thiz, jobject gatt, jobject descriptor, jint status);

    static void jni_onMtuChangedCallback(JNIEnv *env, jobject thiz, jobject gatt, jint mtu, jint status);
    static void jni_onPhyReadCallback(JNIEnv *env, jobject thiz, jobject gatt, jint txPhy, jint rxPhy, jint status);
    static void jni_onPhyUpdateCallback(JNIEnv *env, jobject thiz, jobject gatt, jint txPhy, jint rxPhy, jint status);
    static void jni_onReadRemoteRssiCallback(JNIEnv *env, jobject thiz, jobject gatt, jint rssi, jint status);
    static void jni_onReliableWriteCompletedCallback(JNIEnv *env, jobject thiz, jobject gatt, jint status);
    // clang-format on

  private:
    struct FlagData {
        bool flag;
        std::condition_variable cv;
        std::mutex mtx;
    };


    static JNI::Class _cls;
    static std::map<jobject, BluetoothGattCallback*, JNI::JObjectComparator> _map;
    static void initialize();

    JNI::Object _obj;

    kvn::safe_callback<void(bool)> _callback_onConnectionStateChange;
    kvn::safe_callback<void()> _callback_onServicesDiscovered;

    std::map<JNI::Object, kvn::safe_callback<void(std::vector<uint8_t>)>, JNI::JniObjectComparator> _callback_onCharacteristicChanged;

    std::map<JNI::Object, FlagData, JNI::JniObjectComparator> _flag_descriptorWritePending;
};

}  // namespace Bridge
}  // namespace Android
}  // namespace SimpleBLE