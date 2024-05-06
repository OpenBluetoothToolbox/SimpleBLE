#include "BluetoothDevice.h"

namespace SimpleBLE {
namespace Android {

std::string BluetoothDevice::getAddress() { return _obj.call_string_method("getAddress", "()Ljava/lang/String;"); }

std::string BluetoothDevice::getName() { return _obj.call_string_method("getName", "()Ljava/lang/String;"); }

}  // namespace Android
}  // namespace SimpleBLE

