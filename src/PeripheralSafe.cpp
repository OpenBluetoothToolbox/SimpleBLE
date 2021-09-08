#include <simpleble/PeripheralSafe.h>

#include <simpleble/Exceptions.h>

using namespace SimpleBLE;

PeripheralSafe::PeripheralSafe(Peripheral& peripheral) : Peripheral(peripheral) {}

PeripheralSafe::~PeripheralSafe() {}

std::optional<std::string> PeripheralSafe::identifier() noexcept {
    try {
        return Peripheral::identifier();
    } catch (const Exception::BaseException& e) {
        return std::nullopt;
    }
}

std::optional<BluetoothAddress> PeripheralSafe::address() noexcept {
    try {
        return Peripheral::address();
    } catch (const Exception::BaseException& e) {
        return std::nullopt;
    }
}

bool PeripheralSafe::connect() noexcept {
    try {
        Peripheral::connect();
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool PeripheralSafe::disconnect() noexcept {
    try {
        Peripheral::disconnect();
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

std::optional<bool> PeripheralSafe::is_connected() noexcept {
    try {
        return Peripheral::is_connected();
    } catch (const Exception::BaseException& e) {
        return std::nullopt;
    }
}

std::optional<std::vector<BluetoothService>> PeripheralSafe::services() noexcept {
    try {
        return Peripheral::services();
    } catch (const Exception::BaseException& e) {
        return std::nullopt;
    }
}

std::optional<ByteArray> PeripheralSafe::read(BluetoothUUID service, BluetoothUUID characteristic) noexcept {
    try {
        return Peripheral::read(service, characteristic);
    } catch (const Exception::BaseException& e) {
        return std::nullopt;
    }
}

bool PeripheralSafe::write_request(BluetoothUUID service, BluetoothUUID characteristic, ByteArray data) noexcept {
    try {
        Peripheral::write_request(service, characteristic, data);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool PeripheralSafe::write_command(BluetoothUUID service, BluetoothUUID characteristic, ByteArray data) noexcept {
    try {
        Peripheral::write_command(service, characteristic, data);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool PeripheralSafe::notify(BluetoothUUID service, BluetoothUUID characteristic,
                            std::function<void(ByteArray payload)> callback) noexcept {
    try {
        Peripheral::notify(service, characteristic, callback);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool PeripheralSafe::indicate(BluetoothUUID service, BluetoothUUID characteristic,
                              std::function<void(ByteArray payload)> callback) noexcept {
    try {
        Peripheral::indicate(service, characteristic, callback);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool PeripheralSafe::unsubscribe(BluetoothUUID service, BluetoothUUID characteristic) noexcept {
    try {
        Peripheral::unsubscribe(service, characteristic);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool PeripheralSafe::set_callback_on_connected(std::function<void()> on_connected) noexcept {
    try {
        Peripheral::set_callback_on_connected(on_connected);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}

bool PeripheralSafe::set_callback_on_disconnected(std::function<void()> on_disconnected) noexcept {
    try {
        Peripheral::set_callback_on_disconnected(on_disconnected);
        return true;
    } catch (const Exception::BaseException& e) {
        return false;
    }
}
