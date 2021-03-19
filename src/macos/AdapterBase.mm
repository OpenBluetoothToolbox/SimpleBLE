#import "AdapterBase.h"
#import "AdapterBaseMacOS.h"

#include <thread>
#include <chrono>

using namespace SimpleBLE;

AdapterBase::AdapterBase() {
    opaque_internal_ = [[AdapterBaseMacOS alloc] init];
}

AdapterBase::~AdapterBase() {}

std::vector<std::shared_ptr<AdapterBase> > AdapterBase::get_adapters() {
    // There doesn't seem to be a mechanism with Apple devices that openly
    // exposes more than the default Bluetooth device.
    // For this reason, the MacOS implementation of SimpleBLE will only
    // consider that single case, at least until better alternatives come up.

    std::vector<std::shared_ptr<AdapterBase> > adapter_list;
    adapter_list.push_back(std::make_shared<AdapterBase>());
    return adapter_list;
}

std::string AdapterBase::identifier() { return "Default Adapter"; }

BluetoothAddress AdapterBase::address() { return "00:00:00:00:00:00"; }

void AdapterBase::scan_start() {
    AdapterBaseMacOS* internal = (AdapterBaseMacOS*) opaque_internal_;
    [internal scanStart];

    if (callback_on_scan_start_) {
        callback_on_scan_start_();
    }
}

void AdapterBase::scan_stop() {
    AdapterBaseMacOS* internal = (AdapterBaseMacOS*) opaque_internal_;
    [internal scanStop];
    if (callback_on_scan_stop_) {
        callback_on_scan_stop_();
    }
}

void AdapterBase::scan_for(int timeout_ms) {
    this->scan_start();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
    this->scan_stop();
}

bool AdapterBase::scan_is_active() { return false; }

void AdapterBase::set_callback_on_scan_start(std::function<void()> on_scan_start) {
    callback_on_scan_start_ = on_scan_start;
}
void AdapterBase::set_callback_on_scan_stop(std::function<void()> on_scan_stop) {
    callback_on_scan_stop_ = on_scan_stop;
}
void AdapterBase::set_callback_on_scan_updated(std::function<void(Peripheral)> on_scan_updated) {
    callback_on_scan_updated_ = on_scan_updated;
}
void AdapterBase::set_callback_on_scan_found(std::function<void(Peripheral)> on_scan_found) {
    callback_on_scan_found_ = on_scan_found;
}
