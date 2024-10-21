#include <simplebluez/CustomCharacteristic.h>
#include <simplebluez/Exceptions.h>
#include <simplebluez/CustomService.h>

using namespace SimpleBluez;

CustomService::CustomService(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {

    _interfaces.emplace(std::make_pair("org.bluez.GattService1", std::make_shared<GattService1>(conn, this)));
}

std::shared_ptr<CustomCharacteristic> CustomService::create_characteristic() {
    const std::string characteristic_path = _path + "/characteristic" + std::to_string(_children.size());
    auto characteristic = std::make_shared<CustomCharacteristic>(_conn, _bus_name, characteristic_path);
    path_append_child(characteristic_path, std::static_pointer_cast<SimpleDBus::Proxy>(characteristic));

    characteristic->service(this->path());

    return characteristic;
}

void CustomService::remove_characteristic(const std::string& path) {
    const std::string characteristic_path = _path + "/" + path;
    path_remove_child(characteristic_path);
}

std::shared_ptr<GattService1> CustomService::gattservice1() {
    return std::dynamic_pointer_cast<GattService1>(interface_get("org.bluez.GattService1"));
}

std::string CustomService::uuid() { return gattservice1()->UUID(); }

void CustomService::uuid(const std::string& uuid) { gattservice1()->UUID(uuid); }

bool CustomService::primary() { return gattservice1()->Primary(); }

void CustomService::primary(bool primary) { gattservice1()->Primary(primary); }
