#include "AdapterBase.h"
#include "Bluez.h"

#include <iostream>

using namespace SimpleBLE;

std::vector<std::shared_ptr<AdapterBase>> AdapterBase::get_adapters() {
    std::vector<std::shared_ptr<AdapterBase>> adapter_list;
    auto internal_adapters = Bluez::get()->bluez_service.get_all_adapters();
    for (auto& adapter : internal_adapters) {
        adapter_list.push_back(std::make_shared<AdapterBase>(adapter));
    }
    return adapter_list;
}

AdapterBase::AdapterBase(std::shared_ptr<BluezAdapter> adapter) { adapter_ = adapter; }

AdapterBase::~AdapterBase() {}

std::string AdapterBase::identifier() {
    auto adapter = adapter_.lock();
    if (adapter) {
        return adapter->get_identifier();
    } else {
        throw Exception::InvalidReference();
    }
}

BluetoothAddress AdapterBase::address() {
    auto adapter = adapter_.lock();
    if (adapter) {
        return adapter->Address();
    } else {
        throw Exception::InvalidReference();
    }
}
