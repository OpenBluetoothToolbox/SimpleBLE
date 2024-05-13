#pragma once

#include "BluetoothGatt.h"

#include "jni/Common.hpp"

namespace SimpleBLE {
namespace Android {

class BluetoothDevice {
public:
    BluetoothDevice(JNI::Object obj);

    std::string getAddress();
    std::string getName();

    BluetoothGatt connectGatt(bool autoConnect, JNI::Object callback);

private:
    static JNI::Class _cls;
    static jmethodID _method_getAddress;
    static jmethodID _method_getName;
    static jmethodID _method_connectGatt;

    static void initialize();

    JNI::Object _obj;

};

}  // namespace Android
}  // namespace SimpleBLE
