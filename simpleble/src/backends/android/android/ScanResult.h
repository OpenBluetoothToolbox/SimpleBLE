#pragma once

#include "jni/Common.hpp"

#include "BluetoothDevice.h"

namespace SimpleBLE {
namespace Android {

class ScanResult {
  public:
    ScanResult(JNI::Object obj) : _obj(obj){};

    BluetoothDevice getDevice();
    std::string toString();

  private:
    JNI::Object _obj;
};

}  // namespace Android
}  // namespace SimpleBLE
