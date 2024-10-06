#include <simplebluez/CustomAdvertisementManager.h>
#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleBluez;

CustomAdvertisementManager::CustomAdvertisementManager(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {

    _interfaces.emplace(std::make_pair("org.freedesktop.DBus.ObjectManager", std::make_shared<SimpleDBus::ObjectManager>(conn, this)));
}

std::shared_ptr<SimpleBluez::CustomAdvertisement> CustomAdvertisementManager::create_advertisement(const std::string& path) {
    const std::string advertisement_path = _path + "/" + path;
    auto advertisement = std::make_shared<CustomAdvertisement>(_conn, _bus_name, advertisement_path);
    path_append_child(advertisement_path, std::static_pointer_cast<SimpleDBus::Proxy>(advertisement));
    return advertisement;
}

void CustomAdvertisementManager::remove_advertisement(const std::string& path) {
    const std::string advertisement_path = _path + "/" + path;
    path_remove_child(advertisement_path);
}

std::shared_ptr<SimpleDBus::ObjectManager> CustomAdvertisementManager::object_manager() {
    return std::dynamic_pointer_cast<SimpleDBus::ObjectManager>(interface_get("org.freedesktop.DBus.ObjectManager"));
}
