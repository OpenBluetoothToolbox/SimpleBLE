#include "AdapterBindings.hpp"

#include "simplersble/src/lib.rs.h"

rust::Vec<SimpleBLE::RustyWrapper> RustyAdapter_get_adapters() {
    rust::Vec<SimpleBLE::RustyWrapper> result;

    for (auto &adapter : SimpleBLE::Adapter::get_adapters() ) {
        SimpleBLE::RustyWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyAdapter>(adapter);
        result.push_back(std::move(wrapper));
    }

    return result;
}

bool RustyAdapter_bluetooth_enabled() {
    return SimpleBLE::Adapter::bluetooth_enabled();
}
