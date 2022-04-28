#include <simpleble/AdapterSafe.h>

SimpleBLE::Safe::Adapter::Adapter(SimpleBLE::Adapter& adapter) : SimpleBLE::Adapter(adapter) {}

std::optional<std::string> SimpleBLE::Safe::Adapter::identifier() const noexcept {
    try {
        return SimpleBLE::Adapter::identifier();
    } catch (const SimpleBLE::Exception::BaseException&) {
        return std::nullopt;
    }
}

std::optional<SimpleBLE::BluetoothAddress> SimpleBLE::Safe::Adapter::address() const noexcept {
    try {
        return SimpleBLE::Adapter::address();
    } catch (const SimpleBLE::Exception::BaseException&) {
        return std::nullopt;
    }
}

bool SimpleBLE::Safe::Adapter::scan_start() noexcept {
    try {
        SimpleBLE::Adapter::scan_start();
        return true;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return false;
    }
}

bool SimpleBLE::Safe::Adapter::scan_stop() noexcept {
    try {
        SimpleBLE::Adapter::scan_stop();
        return true;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return false;
    }
}

bool SimpleBLE::Safe::Adapter::scan_for(int timeout_ms) noexcept {
    try {
        SimpleBLE::Adapter::scan_for(timeout_ms);
        return true;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return false;
    }
}

std::optional<bool> SimpleBLE::Safe::Adapter::scan_is_active() const noexcept {
    try {
        return SimpleBLE::Adapter::scan_is_active();
    } catch (const SimpleBLE::Exception::BaseException&) {
        return std::nullopt;
    }
}

std::optional<std::vector<SimpleBLE::Safe::Peripheral>> SimpleBLE::Safe::Adapter::scan_get_results() const noexcept {
    try {
        auto peripherals = SimpleBLE::Adapter::scan_get_results();
        std::vector<SimpleBLE::Safe::Peripheral> safe_peripherals;
        for (auto& peripheral : peripherals) {
            safe_peripherals.push_back(SimpleBLE::Safe::Peripheral(peripheral));
        }
        return safe_peripherals;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return std::nullopt;
    }
    return std::nullopt;
}

std::optional<std::vector<SimpleBLE::Safe::Peripheral>> SimpleBLE::Safe::Adapter::get_paired_peripherals()
    const noexcept {
    try {
        auto peripherals = SimpleBLE::Adapter::get_paired_peripherals();
        std::vector<SimpleBLE::Safe::Peripheral> safe_peripherals;
        for (auto& peripheral : peripherals) {
            safe_peripherals.push_back(SimpleBLE::Safe::Peripheral(peripheral));
        }
        return safe_peripherals;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return std::nullopt;
    }
    return std::nullopt;
}

bool SimpleBLE::Safe::Adapter::set_callback_on_scan_start(std::function<void()> on_scan_start) noexcept {
    try {
        SimpleBLE::Adapter::set_callback_on_scan_start(on_scan_start);
        return true;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return false;
    }
}

bool SimpleBLE::Safe::Adapter::set_callback_on_scan_stop(std::function<void()> on_scan_stop) noexcept {
    try {
        SimpleBLE::Adapter::set_callback_on_scan_stop(on_scan_stop);
        return true;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return false;
    }
}

bool SimpleBLE::Safe::Adapter::set_callback_on_scan_updated(
    std::function<void(SimpleBLE::Safe::Peripheral)> on_scan_updated) noexcept {
    try {
        SimpleBLE::Adapter::set_callback_on_scan_updated(
            [=](SimpleBLE::Peripheral p) { on_scan_updated(SimpleBLE::Safe::Peripheral(p)); });
        return true;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return false;
    }
}

bool SimpleBLE::Safe::Adapter::set_callback_on_scan_found(
    std::function<void(SimpleBLE::Safe::Peripheral)> on_scan_found) noexcept {
    try {
        SimpleBLE::Adapter::set_callback_on_scan_found(
            [=](SimpleBLE::Peripheral p) { on_scan_found(SimpleBLE::Safe::Peripheral(p)); });
        return true;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return false;
    }
}

std::optional<std::vector<SimpleBLE::Safe::Adapter>> SimpleBLE::Safe::Adapter::get_adapters() noexcept {
    try {
        auto adapters = SimpleBLE::Adapter::get_adapters();
        std::vector<SimpleBLE::Safe::Adapter> safe_adapters;
        for (auto& adapter : adapters) {
            safe_adapters.push_back(SimpleBLE::Safe::Adapter(adapter));
        }
        return safe_adapters;
    } catch (const SimpleBLE::Exception::BaseException&) {
        return std::nullopt;
    }
}
