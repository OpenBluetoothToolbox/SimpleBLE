#include <simplebluez/CustomAdvertisement.h>
#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleBluez;

CustomAdvertisement::CustomAdvertisement(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {

    _interfaces.emplace(std::make_pair("org.bluez.LEAdvertisement1", std::make_shared<LEAdvertisement1>(conn, this)));
    _interfaces.emplace(std::make_pair("org.freedesktop.DBus.ObjectManager", std::make_shared<SimpleDBus::ObjectManager>(conn, this)));
}

std::shared_ptr<LEAdvertisement1> CustomAdvertisement::le_advertisement1() {
    return std::dynamic_pointer_cast<LEAdvertisement1>(interface_get("org.bluez.LEAdvertisement1"));
}

std::shared_ptr<SimpleDBus::ObjectManager> CustomAdvertisement::object_manager() {
    return std::dynamic_pointer_cast<SimpleDBus::ObjectManager>(interface_get("org.freedesktop.DBus.ObjectManager"));
}