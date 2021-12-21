#include "AdapterBase.h"
#include "Bluez.h"
#include "PeripheralBase.h"
#include "PeripheralBuilder.h"

using namespace SimpleBLE;

std::vector<std::shared_ptr<AdapterBase>> AdapterBase::get_adapters() {
    std::vector<std::shared_ptr<AdapterBase>> adapter_list;
    auto internal_adapters = Bluez::get()->bluez.get_adapters();
    for (auto& adapter : internal_adapters) {
        adapter_list.push_back(std::make_shared<AdapterBase>(adapter));
    }
    return adapter_list;
}

AdapterBase::AdapterBase(std::shared_ptr<SimpleBluez::Adapter> adapter) { adapter_ = adapter; }

AdapterBase::~AdapterBase() {}

std::string AdapterBase::identifier() { return adapter_->identifier(); }

BluetoothAddress AdapterBase::address() { return adapter_->address(); }

void AdapterBase::scan_start() {
    adapter_->discovery_filter(SimpleBluez::Adapter::DiscoveryFilter::LE);

    this->seen_devices_.clear();
    adapter_->set_on_device_updated([this](std::shared_ptr<SimpleBluez::Device> device) {
        PeripheralBuilder peripheral_builder(std::make_shared<PeripheralBase>(device));

        if (this->seen_devices_.count(peripheral_builder.address()) == 0) {
            this->seen_devices_.insert(std::make_pair<>(peripheral_builder.address(), peripheral_builder));
            if (this->callback_on_scan_found_) {
                this->callback_on_scan_found_(peripheral_builder);
            }
        } else {
            if (this->callback_on_scan_updated_) {
                this->callback_on_scan_updated_(peripheral_builder);
            }
        }
    });

    // Start scanning and notify the user.
    adapter_->discovery_start();
    if (callback_on_scan_start_) {
        callback_on_scan_start_();
    }
}

void AdapterBase::scan_stop() {
    adapter_->discovery_stop();
    // Due to the fact that Bluez takes some time to process the command
    // and for SimpleDBus to flush the queue, wait until the driver
    // has acknowledged that it is no longer discovering.
    while (adapter_->discovering()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (callback_on_scan_stop_) {
        callback_on_scan_stop_();
    }
}

void AdapterBase::scan_for(int timeout_ms) {
    scan_start();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
    scan_stop();
}

bool AdapterBase::scan_is_active() { return adapter_->discovering(); }

std::vector<Peripheral> AdapterBase::scan_get_results() {
    std::vector<Peripheral> peripherals;
    for (auto& [address, peripheral] : this->seen_devices_) {
        peripherals.push_back(peripheral);
    }
    return peripherals;
}

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
