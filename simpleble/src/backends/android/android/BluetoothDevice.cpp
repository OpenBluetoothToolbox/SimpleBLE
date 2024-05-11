#include "BluetoothDevice.h"

namespace SimpleBLE {
namespace Android {

std::string BluetoothDevice::getAddress() { return _obj.call_string_method("getAddress", "()Ljava/lang/String;"); }

std::string BluetoothDevice::getName() { return _obj.call_string_method("getName", "()Ljava/lang/String;"); }

JNI::Object BluetoothDevice::connectGatt(bool autoConnect, JNI::Object callback) {
    return _obj.call_object_method("connectGatt", "(Landroid/content/Context;ZLandroid/bluetooth/BluetoothGattCallback;)Landroid/bluetooth/BluetoothGatt;", nullptr, autoConnect, callback.get());
}

}  // namespace Android
}  // namespace SimpleBLE

