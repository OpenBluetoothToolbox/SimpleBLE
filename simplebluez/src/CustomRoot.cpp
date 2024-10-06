#include <simplebluez/CustomRoot.h>

#include <simplebluez/CustomAdvertisementManager.h>
#include <simplebluez/CustomServiceManager.h>
#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleBluez;

CustomRoot::CustomRoot(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name,
                       const std::string& path)
    : Proxy(conn, bus_name, path) {
    _interfaces.emplace(
        std::make_pair("org.freedesktop.DBus.ObjectManager", std::make_shared<SimpleDBus::ObjectManager>(conn, this)));

    const std::string advertisements_path = path + "/advertisements";
    _custom_advertisements = std::make_shared<CustomAdvertisementManager>(conn, bus_name, advertisements_path);
    path_append_child(advertisements_path, std::static_pointer_cast<SimpleDBus::Proxy>(_custom_advertisements));

    const std::string services_path = path + "/services";
    _custom_services = std::make_shared<CustomServiceManager>(conn, bus_name, services_path);
    path_append_child(services_path, std::static_pointer_cast<SimpleDBus::Proxy>(_custom_services));
}

std::shared_ptr<SimpleDBus::ObjectManager> CustomRoot::object_manager() {
    return std::dynamic_pointer_cast<SimpleDBus::ObjectManager>(interface_get("org.freedesktop.DBus.ObjectManager"));
}

std::shared_ptr<CustomAdvertisementManager> CustomRoot::get_custom_advertisements() {
    return _custom_advertisements;
}

std::shared_ptr<CustomServiceManager> CustomRoot::get_custom_services() {
    return _custom_services;
}
