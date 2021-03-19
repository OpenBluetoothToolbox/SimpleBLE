#import "PeripheralBase.h"

using namespace SimpleBLE;

PeripheralBase::PeripheralBase() {}

PeripheralBase::~PeripheralBase() {}

std::string PeripheralBase::identifier() { return "Default Adapter"; }

BluetoothAddress PeripheralBase::address() { return "00:00:00:00:00:00"; }