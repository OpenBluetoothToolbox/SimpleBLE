#pragma once

#include "simpleble/Adapter.h"

#include <cstdint>
#include <memory>

bool Adapter_bluetooth_enabled() {
    return SimpleBLE::Adapter::bluetooth_enabled();
}

int32_t Adapter_get_adapters_count() {
    return SimpleBLE::Adapter::get_adapters().size();
}

std::unique_ptr<SimpleBLE::Adapter> Adapter_get_adapters_from_index(int32_t index) {
    return std::make_unique<SimpleBLE::Adapter>(SimpleBLE::Adapter::get_adapters()[index]);
}
