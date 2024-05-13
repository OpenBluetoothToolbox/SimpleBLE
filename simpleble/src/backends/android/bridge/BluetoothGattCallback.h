#pragma once

#include "jni/Common.hpp"

namespace SimpleBLE {
namespace Android {
namespace Bridge {

class BluetoothGattCallback {
  public:
    BluetoothGattCallback();

  private:
    static JNI::Class _cls;

    static void initialize();

    JNI::Object _obj;
};

}  // namespace Bridge
}  // namespace Android
}  // namespace SimpleBLE