#include <simplebluez/LEAdvertisement.h>

using namespace SimpleBluez;

LEAdvertisement::LEAdvertisement(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name,
                                 const std::string& path)
    : Proxy(conn, bus_name, path) {
    _interfaces.emplace(std::make_pair("org.bluez.LEAdvertisement1", interfaces_create("org.bluez.LEAdvertisement1")));
}

std::shared_ptr<SimpleDBus::Interface> LEAdvertisement::interfaces_create(const std::string& interface_name) {
    if (interface_name == "org.bluez.LEAdvertisement1") {
        return std::static_pointer_cast<SimpleDBus::Interface>(std::make_shared<LEAdvertisement1>(_conn, _path));
    }

    auto interface = std::make_shared<SimpleDBus::Interface>(_conn, _bus_name, _path, interface_name);
    return std::static_pointer_cast<SimpleDBus::Interface>(interface);
}
