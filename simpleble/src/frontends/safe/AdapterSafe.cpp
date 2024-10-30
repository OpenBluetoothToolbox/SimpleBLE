#include <simpleble/AdapterSafe.h>

#include "BuildVec.h"

using namespace SimpleBLE::Safe;

using SPeripheral = SimpleBLE::Safe::Peripheral;
using SAdapter = SimpleBLE::Safe::Adapter;

std::optional<std::string> SAdapter::identifier() const noexcept {
    try {
        return internal_.identifier();
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<SimpleBLE::BluetoothAddress> SAdapter::address() noexcept {
    try {
        return internal_.address();
    } catch (...) {
        return std::nullopt;
    }
}

bool SAdapter::scan_start() noexcept {
    try {
        internal_.scan_start();
        return true;
    } catch (...) {
        return false;
    }
}

bool SAdapter::scan_stop() noexcept {
    try {
        internal_.scan_stop();
        return true;
    } catch (...) {
        return false;
    }
}

bool SAdapter::scan_for(int timeout_ms) noexcept {
    try {
        internal_.scan_for(timeout_ms);
        return true;
    } catch (...) {
        return false;
    }
}

std::optional<bool> SAdapter::scan_is_active() noexcept {
    try {
        return internal_.scan_is_active();
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<std::vector<SPeripheral>> SAdapter::scan_get_results() noexcept {
    try {
        return (std::vector<SPeripheral>)build_vec(internal_.scan_get_results());
    } catch (...) {
        return std::nullopt;
    }
    return std::nullopt;
}

std::optional<std::vector<SPeripheral>> SAdapter::get_paired_peripherals() noexcept {
    try {
        return (std::vector<SPeripheral>)build_vec(internal_.get_paired_peripherals());
    } catch (...) {
        return std::nullopt;
    }
    return std::nullopt;
}

bool SAdapter::set_callback_on_scan_start(std::function<void()> on_scan_start) noexcept {
    try {
        internal_.set_callback_on_scan_start(std::move(on_scan_start));
        return true;
    } catch (...) {
        return false;
    }
}

bool SAdapter::set_callback_on_scan_stop(std::function<void()> on_scan_stop) noexcept {
    try {
        internal_.set_callback_on_scan_stop(std::move(on_scan_stop));
        return true;
    } catch (...) {
        return false;
    }
}

bool SAdapter::set_callback_on_scan_updated(std::function<void(SPeripheral)> on_scan_updated) noexcept {
    try {
        internal_.set_callback_on_scan_updated(
            [on_scan_updated = std::move(on_scan_updated)](auto p) { on_scan_updated(build(p)); });
        return true;
    } catch (...) {
        return false;
    }
}

bool SAdapter::set_callback_on_scan_found(std::function<void(SPeripheral)> on_scan_found) noexcept {
    try {
        internal_.set_callback_on_scan_found(
            [on_scan_found = std::move(on_scan_found)](auto p) { on_scan_found(build(p)); });
        return true;
    } catch (...) {
        return false;
    }
}

std::optional<bool> SAdapter::bluetooth_enabled() noexcept {
    try {
        return internal_.bluetooth_enabled();
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<std::vector<SAdapter>> SAdapter::get_adapters() noexcept {
    try {
        auto adapters = SimpleBLE::Adapter::get_adapters();
        std::vector<SAdapter> safe_adapters;
        for (auto& adapter : adapters) {
            safe_adapters.push_back(build(std::move(adapter)));
        }
        return safe_adapters;
    } catch (...) {
        return std::nullopt;
    }
}
