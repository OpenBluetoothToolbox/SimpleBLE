#include <simpleble/Adapter.h>

#include "AdapterBase.h"

#include <iostream>

using namespace SimpleBLE;

Adapter::Adapter() {}

Adapter::~Adapter() {}

std::string Adapter::identifier() { return internal_->identifier(); }

BluetoothAddress Adapter::address() { return internal_->address(); }

std::vector<Adapter> Adapter::get_adapters() {
    std::vector<Adapter> available_adapters;
    auto internal_adapters = AdapterBase::get_adapters();

    for (auto& internal_adapter : internal_adapters) {
        Adapter adapter;
        // Set the internal pointer within this function,
        // to avoid the need to expose the parameter through a constructor.
        adapter.internal_ = internal_adapter;
        available_adapters.push_back(adapter);
    }

    return available_adapters;
}