#pragma once

#include "simpleble/Adapter.h"
#include "rust/cxx.h"

#include <cstdint>
#include <memory>
#include <iostream>

namespace SimpleBLE {

struct RustyWrapper;

class RustyAdapter : private Adapter {
  public:
    RustyAdapter() = default;
    virtual ~RustyAdapter() = default;

    RustyAdapter(Adapter adapter) : _adapter(new Adapter(adapter)) {
        std::cout << "Passing an adapter in!\n";
    };

    rust::String identifier() const {
        return rust::String(_adapter->identifier());
    }

  private:
    std::shared_ptr<Adapter> _adapter;
};

};

rust::Vec<SimpleBLE::RustyWrapper> RustyAdapter_get_adapters();


bool RustyAdapter_bluetooth_enabled() {
    return SimpleBLE::Adapter::bluetooth_enabled();
}
