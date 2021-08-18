#import "PeripheralBase.h"
#import "PeripheralBaseMacOS.h"

#include <iostream>

using namespace SimpleBLE;

PeripheralBase::PeripheralBase(void* opaque_peripheral, void* opaque_adapter, advertising_data_t advertising_data) {
    this->opaque_internal_ = [[PeripheralBaseMacOS alloc] init:(CBPeripheral*)opaque_peripheral
                                                centralManager:(CBCentralManager*)opaque_adapter];
    // TODO: Set advertising data
}

PeripheralBase::~PeripheralBase() {}

std::string PeripheralBase::identifier() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    return std::string([[internal identifier] UTF8String]);
}

BluetoothAddress PeripheralBase::address() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    return std::string([[internal address] UTF8String]);
}

void PeripheralBase::connect() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    [internal connect];
    // TODO: Call callback
}

void PeripheralBase::disconnect() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    [internal disconnect];
    // TODO: Call callback
}

bool PeripheralBase::is_connected() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    return [internal isConnected];
}

std::vector<BluetoothService> PeripheralBase::services() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    return [internal getServices];
}

void PeripheralBase::set_callback_on_connected(std::function<void()> on_connected) { callback_on_connected_ = on_connected; }

void PeripheralBase::set_callback_on_disconnected(std::function<void()> on_disconnected) { callback_on_disconnected_ = on_disconnected; }

void PeripheralBase::delegate_did_connect() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    [internal delegateDidConnect];
}

void PeripheralBase::delegate_did_disconnect() {
    PeripheralBaseMacOS* internal = (PeripheralBaseMacOS*)opaque_internal_;
    [internal delegateDidDisconnect];
}
