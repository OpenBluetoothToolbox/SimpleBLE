#include <simplebluez/CustomServiceManager.h>
#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleBluez;

CustomServiceManager::CustomServiceManager(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {

    _interfaces.emplace(std::make_pair("org.freedesktop.DBus.ObjectManager", std::make_shared<SimpleDBus::ObjectManager>(conn, this)));
}

std::shared_ptr<SimpleBluez::CustomService> CustomServiceManager::create_service() {
    const std::string service_path = _path + "/service" + std::to_string(_children.size());
    auto service = std::make_shared<CustomService>(_conn, _bus_name, service_path);
    path_append_child(service_path, std::static_pointer_cast<SimpleDBus::Proxy>(service));
    return service;
}

void CustomServiceManager::remove_service(const std::string& path) {
    const std::string service_path = _path + "/" + path;
    path_remove_child(service_path);
}

std::shared_ptr<SimpleDBus::ObjectManager> CustomServiceManager::object_manager() {
    return std::dynamic_pointer_cast<SimpleDBus::ObjectManager>(interface_get("org.freedesktop.DBus.ObjectManager"));
}
