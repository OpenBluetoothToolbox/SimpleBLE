#include "BluetoothGattDescriptor.h"
#include "UUID.h"

namespace SimpleBLE {
namespace Android {

JNI::Class BluetoothGattDescriptor::_cls;
jmethodID BluetoothGattDescriptor::_method_getCharacteristic = nullptr;
jmethodID BluetoothGattDescriptor::_method_getPermissions = nullptr;
jmethodID BluetoothGattDescriptor::_method_getUuid = nullptr;

void BluetoothGattDescriptor::initialize() {
    JNI::Env env;

    if (_cls.get() == nullptr) {
        _cls = env.find_class("android/bluetooth/BluetoothGattDescriptor");
    }

    if (!_method_getCharacteristic) {
        _method_getCharacteristic = env->GetMethodID(_cls.get(), "getCharacteristic",
                                                     "()Landroid/bluetooth/BluetoothGattCharacteristic;");
    }

    if (!_method_getPermissions) {
        _method_getPermissions = env->GetMethodID(_cls.get(), "getPermissions", "()I");
    }

    if (!_method_getUuid) {
        _method_getUuid = env->GetMethodID(_cls.get(), "getUuid", "()Ljava/util/UUID;");
    }
}

BluetoothGattDescriptor::BluetoothGattDescriptor() { initialize(); }

BluetoothGattDescriptor::BluetoothGattDescriptor(JNI::Object obj) : BluetoothGattDescriptor() { _obj = obj; }

int BluetoothGattDescriptor::getPermissions() { return _obj.call_int_method(_method_getPermissions); }

std::string BluetoothGattDescriptor::getUuid() {
    if (!_obj) return "";

    JNI::Object uuidObj = _obj.call_object_method(_method_getUuid);
    if (!uuidObj) return "";

    return UUID(uuidObj).toString();
}
}  // namespace Android
}  // namespace SimpleBLE