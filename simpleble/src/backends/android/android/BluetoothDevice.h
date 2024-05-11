#pragma once

#include "jni/Common.hpp"

namespace SimpleBLE {
namespace Android {

class BluetoothDevice {
public:
    BluetoothDevice(JNI::Object obj) : _obj(obj) {};

    std::string getAddress();
    std::string getName();

    JNI::Object connectGatt(bool autoConnect, JNI::Object callback);

private:
    JNI::Object _obj;

};

}  // namespace Android
}  // namespace SimpleBLE
