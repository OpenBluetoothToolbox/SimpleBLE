#include <simpleble/Adapter.h>

#include "AdapterBase.h"
#include "AdapterBuilder.h"
#include "LoggingInternal.h"

using namespace SimpleBLE;

std::vector<Adapter> Adapter::get_adapters() {
    std::vector<Adapter> available_adapters;
    auto internal_adapters = AdapterBase::get_adapters();

    for (auto& internal_adapter : internal_adapters) {
        AdapterBuilder adapter(internal_adapter);
        available_adapters.push_back(adapter);
    }

    return available_adapters;
}

bool Adapter::bluetooth_enabled() { return AdapterBase::bluetooth_enabled(); }

bool Adapter::initialized() const { return internal_ != nullptr; }

void* Adapter::underlying() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().underlying();
}

std::string Adapter::identifier() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().identifier();
}

BluetoothAddress Adapter::address() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().address();
}

void Adapter::scan_start() {
    if (!initialized()) throw Exception::NotInitialized();
    if (!bluetooth_enabled()) {
        SIMPLEBLE_LOG_WARN(fmt::format("Bluetooth is not enabled."));
        return;
    }
    internal().scan_start();
}

void Adapter::scan_stop() {
    if (!initialized()) throw Exception::NotInitialized();
    if (!bluetooth_enabled()) {
        SIMPLEBLE_LOG_WARN(fmt::format("Bluetooth is not enabled."));
        return;
    }
    internal().scan_stop();
}

void Adapter::scan_for(int timeout_ms) {
    if (!initialized()) throw Exception::NotInitialized();
    if (!bluetooth_enabled()) {
        SIMPLEBLE_LOG_WARN(fmt::format("Bluetooth is not enabled."));
        return;
    }
    internal().scan_for(timeout_ms);
}

bool Adapter::scan_is_active() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().scan_is_active();
}

std::vector<Peripheral> Adapter::scan_get_results() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().scan_get_results();
}

std::vector<Peripheral> Adapter::get_paired_peripherals() const {
    if (!initialized()) throw Exception::NotInitialized();

    return internal().get_paired_peripherals();
}

void Adapter::set_callback_on_scan_start(std::function<void()> on_scan_start) {
    if (!initialized()) throw Exception::NotInitialized();

    internal().set_callback_on_scan_start(std::move(on_scan_start));
}

void Adapter::set_callback_on_scan_stop(std::function<void()> on_scan_stop) {
    if (!initialized()) throw Exception::NotInitialized();

    internal().set_callback_on_scan_stop(std::move(on_scan_stop));
}

void Adapter::set_callback_on_scan_updated(std::function<void(Peripheral)> on_scan_updated) {
    if (!initialized()) throw Exception::NotInitialized();

    internal().set_callback_on_scan_updated(std::move(on_scan_updated));
}

void Adapter::set_callback_on_scan_found(std::function<void(Peripheral)> on_scan_found) {
    if (!initialized()) throw Exception::NotInitialized();

    internal().set_callback_on_scan_found(std::move(on_scan_found));
}

AdapterBase &Adapter::internal() {
    return *internal_;
}

const AdapterBase &Adapter::internal() const {
    return *internal_;
}
