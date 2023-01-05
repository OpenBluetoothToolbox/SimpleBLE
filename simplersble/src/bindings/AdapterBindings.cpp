#include "AdapterBindings.hpp"

#include "simplersble/simplersble/src/lib.rs.h"

// Adapter Bindings

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


void SimpleBLE::RustyAdapter::link(SimpleRsBLE::Adapter &target) const {
    // Time to explain the weird shenanigan we're doing here:
    // The TL;DR is that we're making the Adapter(Rust) and the RustyAdapter(C++)
    // point to each other in a safe way.
    // To achieve this, the Adapter(Rust) owns a RustyAdapter(C++) via a UniquePtr,
    // which ensures that calls will always be made to a valid C++ object.
    // We now give the RustyAdapter(C++) a pointer back to the Adapter(Rust),
    // so that callbacks can be forwarded back to the Rust domain.
    // In order to ensure that the Adapter(Rust) is always valid (given
    // that Rust is keen on moving stuff around) the object is created as a
    // Pin<Box<T>>


    // `_adapter` is a pointer to a pointer, allowing us to manipulate the contents within const functions.
    *_adapter = &target; // THIS LINE IS SUPER IMPORTANT

    _internal->set_callback_on_scan_start([this]() {
        SimpleRsBLE::Adapter* p_adapter = *this->_adapter;
        if (p_adapter == nullptr) return;

        p_adapter->on_callback_scan_start();
    });

    _internal->set_callback_on_scan_stop([this]() {
        SimpleRsBLE::Adapter* p_adapter = *this->_adapter;
        if (p_adapter == nullptr) return;

        p_adapter->on_callback_scan_stop();
    });

    _internal->set_callback_on_scan_found([this](SimpleBLE::Peripheral peripheral) {
        SimpleRsBLE::Adapter* p_adapter = *this->_adapter;
        if (p_adapter == nullptr) return;

        SimpleBLE::RustyPeripheralWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyPeripheral>(peripheral);
        p_adapter->on_callback_scan_found(wrapper);
    });

    _internal->set_callback_on_scan_updated([this](SimpleBLE::Peripheral peripheral) {
        SimpleRsBLE::Adapter* p_adapter = *this->_adapter;
        if (p_adapter == nullptr) return;

        SimpleBLE::RustyPeripheralWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyPeripheral>(peripheral);
        p_adapter->on_callback_scan_updated(wrapper);
    });
}
void SimpleBLE::RustyAdapter::unlink() const {
    // `_adapter` is a pointer to a pointer.
    *_adapter = nullptr;
}

rust::String SimpleBLE::RustyAdapter::identifier() const { return rust::String(_internal->identifier()); }

rust::String SimpleBLE::RustyAdapter::address() const { return rust::String(_internal->address()); }

void SimpleBLE::RustyAdapter::scan_start() const { _internal->scan_start(); }

void SimpleBLE::RustyAdapter::scan_stop() const { _internal->scan_stop(); }

void SimpleBLE::RustyAdapter::scan_for(int32_t timeout_ms) const { _internal->scan_for(timeout_ms); }

bool SimpleBLE::RustyAdapter::scan_is_active() const { return _internal->scan_is_active(); }

rust::Vec<SimpleBLE::RustyPeripheralWrapper> SimpleBLE::RustyAdapter::scan_get_results() const {
    rust::Vec<SimpleBLE::RustyPeripheralWrapper> result;

    for (auto& peripheral : _internal->scan_get_results()) {
        SimpleBLE::RustyPeripheralWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyPeripheral>(peripheral);
        result.push_back(std::move(wrapper));
    }

    return result;
}


// Peripheral Bindings

void SimpleBLE::RustyPeripheral::link(SimpleRsBLE::Peripheral &target) const {
    // Time to explain the weird shenanigan we're doing here:
    // The TL;DR is that we're making the Peripheral(Rust) and the RustyPeripheral(C++)
    // point to each other in a safe way.
    // To achieve this, the Peripheral(Rust) owns a RustyPeripheral(C++) via a UniquePtr,
    // which ensures that calls will always be made to a valid C++ object.
    // We now give the RustyPeripheral(C++) a pointer back to the Peripheral(Rust),
    // so that callbacks can be forwarded back to the Rust domain.
    // In order to ensure that the Peripheral(Rust) is always valid (given
    // that Rust is keen on moving stuff around) the object is created as a
    // Pin<Box<T>>


    // `_peripheral` is a pointer to a pointer, allowing us to manipulate the contents within const functions.
    *_peripheral = &target; // THIS LINE IS SUPER IMPORTANT
}
void SimpleBLE::RustyPeripheral::unlink() const {
    // `_peripheral` is a pointer to a pointer.
    *_peripheral = nullptr;
}