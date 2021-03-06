#import "AdapterBaseMacOS.h"

#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>

using namespace SimpleBLE;

AdapterBaseMacOS::AdapterBaseMacOS() {}

AdapterBaseMacOS::~AdapterBaseMacOS() {}

std::string AdapterBaseMacOS::identifier() {
    return "Default Adapter";
}

BluetoothAddress AdapterBaseMacOS::address() {
    return "00:00:00:00:00:00";
}
