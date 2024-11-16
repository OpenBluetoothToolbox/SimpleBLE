#include "AdapterPlain.h"
#include "CommonUtils.h"
#include "PeripheralPlain.h"
#include "PeripheralBuilder.h"

#include <thread>

using namespace SimpleBLE;

std::vector<std::shared_ptr<AdapterBase>> AdapterPlain::get_adapters() {
    std::vector<std::shared_ptr<AdapterBase>> adapter_list;
    adapter_list.push_back(std::static_pointer_cast<AdapterBase>(std::make_shared<AdapterPlain>()));
    return adapter_list;
}

bool AdapterPlain::bluetooth_enabled() { return true; }

AdapterPlain::AdapterPlain() {}

AdapterPlain::~AdapterPlain() {}

void* AdapterPlain::underlying() const { return nullptr; }

std::string AdapterPlain::identifier() { return "Plain Adapter"; }

BluetoothAddress AdapterPlain::address() { return "AA:BB:CC:DD:EE:FF"; }

void AdapterPlain::scan_start() {
    is_scanning_ = true;
    SAFE_CALLBACK_CALL(callback_on_scan_start_);

    PeripheralBuilder peripheral_builder(std::make_shared<PeripheralPlain>());
    SAFE_CALLBACK_CALL(callback_on_scan_found_, peripheral_builder);
    SAFE_CALLBACK_CALL(callback_on_scan_updated_, peripheral_builder);
}

void AdapterPlain::scan_stop() {
    is_scanning_ = false;
    SAFE_CALLBACK_CALL(callback_on_scan_stop_);
}

void AdapterPlain::scan_for(int timeout_ms) {
    scan_start();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
    scan_stop();
}

bool AdapterPlain::scan_is_active() { return is_scanning_; }

std::vector<Peripheral> AdapterPlain::scan_get_results() {
    std::vector<Peripheral> peripherals;

    PeripheralBuilder peripheral_builder(std::static_pointer_cast<PeripheralBase>(std::make_shared<PeripheralPlain>()));
    peripherals.push_back(peripheral_builder);

    return peripherals;
}

std::vector<Peripheral> AdapterPlain::get_paired_peripherals() {
    std::vector<Peripheral> peripherals;

    PeripheralBuilder peripheral_builder(std::static_pointer_cast<PeripheralBase>(std::make_shared<PeripheralPlain>()));
    peripherals.push_back(peripheral_builder);

    return peripherals;
}

void AdapterPlain::set_callback_on_scan_start(std::function<void()> on_scan_start) {
    if (on_scan_start) {
        callback_on_scan_start_.load(std::move(on_scan_start));
    } else {
        callback_on_scan_start_.unload();
    }
}

void AdapterPlain::set_callback_on_scan_stop(std::function<void()> on_scan_stop) {
    if (on_scan_stop) {
        callback_on_scan_stop_.load(std::move(on_scan_stop));
    } else {
        callback_on_scan_stop_.unload();
    }
}

void AdapterPlain::set_callback_on_scan_updated(std::function<void(Peripheral)> on_scan_updated) {
    if (on_scan_updated) {
        callback_on_scan_updated_.load(std::move(on_scan_updated));
    } else {
        callback_on_scan_updated_.unload();
    }
}

void AdapterPlain::set_callback_on_scan_found(std::function<void(Peripheral)> on_scan_found) {
    if (on_scan_found) {
        callback_on_scan_found_.load(std::move(on_scan_found));
    } else {
        callback_on_scan_found_.unload();
    }
}
