#pragma once

#include <string>
#include "jni/Common.hpp"

namespace SimpleBLE {
namespace Android {

class BluetoothGattDescriptor {
  public:
    BluetoothGattDescriptor();
    BluetoothGattDescriptor(JNI::Object obj);

    int getPermissions();
    std::string getUuid();

    JNI::Object getObject() const { return _obj; }

  private:
    JNI::Object _obj;
    static JNI::Class _cls;
    static jmethodID _method_getCharacteristic;
    static jmethodID _method_getPermissions;
    static jmethodID _method_getUuid;

    void initialize();
};

}  // namespace Android
}  // namespace SimpleBLE
