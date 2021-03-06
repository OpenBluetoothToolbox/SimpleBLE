#import "AdapterBase.h"
#include "AdapterBaseMacOS.h"

using namespace SimpleBLE;

AdapterBase::AdapterBase() {}
AdapterBase::~AdapterBase() {}

std::vector<std::shared_ptr<AdapterBase> > AdapterBase::get_adapters() {
    // There doesn't seem to be a mechanism with Apple devices that openly
    // exposes more than the default Bluetooth device.
    // For this reason, the MacOS implementation of SimpleBLE will only
    // consider that single case, at least until better alternatives come up.

    std::vector<std::shared_ptr<AdapterBase> > adapter_list;
    adapter_list.push_back(std::make_shared<AdapterBaseMacOS>());
    return adapter_list;
}
