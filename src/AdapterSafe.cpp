#include <simpleble/AdapterSafe.h>

using namespace SimpleBLE;

AdapterSafe::AdapterSafe(Adapter& adapter) : Adapter(adapter) {}

AdapterSafe::~AdapterSafe() {}

std::optional<std::string> AdapterSafe::identifier() noexcept {
    try {
        return Adapter::identifier();
    } catch (const Exception::BaseException& e) {
        return std::nullopt;
    }
}

std::optional<BluetoothAddress> AdapterSafe::address() noexcept {
    try {
        return Adapter::address();
    } catch (const Exception::BaseException& e) {
        return std::nullopt;
    }
}

bool AdapterSafe::scan_start() noexcept {
    try {
        Adapter::scan_start();
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool AdapterSafe::scan_stop() noexcept {
    try {
        Adapter::scan_stop();
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool AdapterSafe::scan_for(int timeout_ms) noexcept {
    try {
        Adapter::scan_for(timeout_ms);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

std::optional<bool> AdapterSafe::scan_is_active() noexcept {
    try {
        return Adapter::scan_is_active();
    } catch (const Exception::BaseException& e) {
        return std::nullopt;
    }
}

std::optional<std::vector<PeripheralSafe>> AdapterSafe::scan_get_results() noexcept {
    // TODO: Implement
    return std::nullopt;
}

bool AdapterSafe::set_callback_on_scan_start(std::function<void()> on_scan_start) noexcept {
    try {
        Adapter::set_callback_on_scan_start(on_scan_start);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool AdapterSafe::set_callback_on_scan_stop(std::function<void()> on_scan_stop) noexcept {
    try {
        Adapter::set_callback_on_scan_stop(on_scan_stop);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool AdapterSafe::set_callback_on_scan_updated(std::function<void(PeripheralSafe)> on_scan_updated) noexcept {
    try {
        Adapter::set_callback_on_scan_updated([=](Peripheral p) { on_scan_updated(PeripheralSafe(p)); });
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool AdapterSafe::set_callback_on_scan_found(std::function<void(PeripheralSafe)> on_scan_found) noexcept {
    try {
        Adapter::set_callback_on_scan_updated([=](Peripheral p) { on_scan_found(PeripheralSafe(p)); });
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

std::optional<std::vector<AdapterSafe>> AdapterSafe::get_adapters() noexcept {
    try {
        auto adapters = Adapter::get_adapters();
        std::vector<AdapterSafe> safe_adapters;
        for (auto& adapter : adapters) {
            safe_adapters.push_back(AdapterSafe(adapter));
        }
        return safe_adapters;
    } catch (const Exception::BaseException& e) {
        return std::nullopt;
    }
}
