#include <simplebluez/CustomCharacteristic.h>
#include <simplebluez/Exceptions.h>
#include <simplebluez/CustomService.h>

using namespace SimpleBluez;

CustomService::CustomService(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {}

std::shared_ptr<SimpleDBus::Proxy> CustomService::path_create(const std::string& path) {
    auto child = std::make_shared<CustomCharacteristic>(_conn, _bus_name, path);
    return std::static_pointer_cast<SimpleDBus::Proxy>(child);
}

std::shared_ptr<SimpleDBus::Interface> CustomService::interfaces_create(const std::string& interface_name) {
    if (interface_name == "org.bluez.GattService1") {
        return std::static_pointer_cast<SimpleDBus::Interface>(std::make_shared<GattService1>(_conn, this));
    }

    return std::make_shared<SimpleDBus::Interface>(_conn, this, interface_name);
}

std::shared_ptr<GattService1> CustomService::gattservice1() {
    return std::dynamic_pointer_cast<GattService1>(interface_get("org.bluez.GattService1"));
}

std::vector<std::shared_ptr<CustomCharacteristic>> CustomService::characteristics() { return children_casted<CustomCharacteristic>(); }

std::shared_ptr<CustomCharacteristic> CustomService::get_characteristic(const std::string& uuid) {
    auto characteristics_all = characteristics();

    for (auto& characteristic : characteristics_all) {
        if (characteristic->uuid() == uuid) {
            return characteristic;
        }
    }

    throw Exception::CharacteristicNotFoundException(uuid);
}

std::string CustomService::uuid() { return gattservice1()->UUID(); }
