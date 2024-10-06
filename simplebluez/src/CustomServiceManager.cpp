#include <simplebluez/CustomServiceManager.h>
#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleBluez;

CustomServiceManager::CustomServiceManager(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {

    _interfaces.emplace(std::make_pair("org.freedesktop.DBus.ObjectManager", std::make_shared<SimpleDBus::ObjectManager>(conn, this)));

}

std::shared_ptr<SimpleDBus::ObjectManager> CustomServiceManager::object_manager() {
    return std::dynamic_pointer_cast<SimpleDBus::ObjectManager>(interface_get("org.freedesktop.DBus.ObjectManager"));
}
