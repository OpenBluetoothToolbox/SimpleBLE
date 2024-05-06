
#include "ScanResult.h"

namespace SimpleBLE {
namespace Android {

BluetoothDevice ScanResult::getDevice() {
    return BluetoothDevice(_obj.call_object_method("getDevice", "()Landroid/bluetooth/BluetoothDevice;"));
}

std::string ScanResult::toString() {
    return _obj.call_string_method("toString", "()Ljava/lang/String;");
}

}  // namespace Android
}  // namespace SimpleBLE