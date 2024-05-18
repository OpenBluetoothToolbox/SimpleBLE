#include "BluetoothGatt.h"

namespace SimpleBLE {
namespace Android {

JNI::Class BluetoothGatt::_cls;
jmethodID BluetoothGatt::_method_close = nullptr;
jmethodID BluetoothGatt::_method_connect = nullptr;
jmethodID BluetoothGatt::_method_disconnect = nullptr;
jmethodID BluetoothGatt::_method_discoverServices = nullptr;

void BluetoothGatt::initialize() {
    JNI::Env env;

    if (_cls.get() == nullptr) {
        _cls = env.find_class("android/bluetooth/BluetoothGatt");
    }

    if (!_method_close) {
        _method_close = env->GetMethodID(_cls.get(), "close", "()V");
    }

    if (!_method_connect) {
        _method_connect = env->GetMethodID(_cls.get(), "connect", "()Z");
    }

    if (!_method_disconnect) {
        _method_disconnect = env->GetMethodID(_cls.get(), "disconnect", "()V");
    }

    if (!_method_discoverServices) {
        _method_discoverServices = env->GetMethodID(_cls.get(), "discoverServices", "()Z");
    }
}

BluetoothGatt::BluetoothGatt() {
    initialize();
}

BluetoothGatt::BluetoothGatt(JNI::Object obj) : BluetoothGatt() {
    _obj = obj;
}

void BluetoothGatt::close() {
    if (!_obj) return;

    _obj.call_void_method(_method_close);
}

bool BluetoothGatt::connect() {
    if (!_obj) return false;

    return _obj.call_boolean_method(_method_connect);
}

void BluetoothGatt::disconnect() {
    if (!_obj) return;

    _obj.call_void_method(_method_disconnect);
}

bool BluetoothGatt::discoverServices() {
    if (!_obj) return false;

    return _obj.call_boolean_method(_method_discoverServices);
}



}  // namespace Android
}  // namespace SimpleBLE