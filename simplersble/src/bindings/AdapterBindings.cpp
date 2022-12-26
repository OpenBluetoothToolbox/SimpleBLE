#include "AdapterBindings.hpp"

#include "simplersble/src/lib.rs.h"

rust::Vec<SimpleBLE::RustyAdapterWrapper> RustyAdapter_get_adapters() {
    rust::Vec<SimpleBLE::RustyAdapterWrapper> result;

    for (auto& adapter : SimpleBLE::Adapter::get_adapters()) {
        SimpleBLE::RustyAdapterWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyAdapter>(adapter);
        result.push_back(std::move(wrapper));
    }

    return result;
}

bool RustyAdapter_bluetooth_enabled() { return SimpleBLE::Adapter::bluetooth_enabled(); }

rust::String SimpleBLE::RustyAdapter::identifier() const { return rust::String(_adapter->identifier()); }

rust::String SimpleBLE::RustyAdapter::address() const { return rust::String(_adapter->address()); }

void SimpleBLE::RustyAdapter::scan_start() const { _adapter->scan_start(); }

void SimpleBLE::RustyAdapter::scan_stop() const { _adapter->scan_stop(); }

void SimpleBLE::RustyAdapter::scan_for(int32_t timeout_ms) const { _adapter->scan_for(timeout_ms); }

bool SimpleBLE::RustyAdapter::scan_is_active() const { return _adapter->scan_is_active(); }

rust::Vec<SimpleBLE::RustyPeripheralWrapper> SimpleBLE::RustyAdapter::scan_get_results() const {
    rust::Vec<SimpleBLE::RustyPeripheralWrapper> result;

    for (auto& peripheral : _adapter->scan_get_results()) {
        SimpleBLE::RustyPeripheralWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyPeripheral>(peripheral);
        result.push_back(std::move(wrapper));
    }

    return result;
}
