#include <simplebluez/BluezRoot.h>
#include <simplebluez/BluezOrg.h>
#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleBluez;

BluezRoot::BluezRoot(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {

    _interfaces.emplace(std::make_pair("org.freedesktop.DBus.ObjectManager", std::make_shared<SimpleDBus::ObjectManager>(conn, this)));

    object_manager()->InterfacesAdded = [&](std::string path, SimpleDBus::Holder options) { path_add(path, options); };
    object_manager()->InterfacesRemoved = [&](std::string path, SimpleDBus::Holder options) {
        path_remove(path, options);
    };

    // Create the agent that will handle pairing.
    _agent = std::make_shared<Agent>(_conn, "org.bluez", "/agent");
    _agent->init();
    path_append_child("/agent", std::static_pointer_cast<SimpleDBus::Proxy>(_agent));
}

void BluezRoot::load_managed_objects() {
    SimpleDBus::Holder managed_objects = object_manager()->GetManagedObjects();
    for (auto& [path, managed_interfaces] : managed_objects.get_dict_object_path()) {
        path_add(path, managed_interfaces);
    }
}

std::vector<std::shared_ptr<Adapter>> BluezRoot::get_adapters() {
    return std::dynamic_pointer_cast<BluezOrg>(path_get("/org"))->get_adapters();
}

std::shared_ptr<Agent> BluezRoot::get_agent() { return std::dynamic_pointer_cast<Agent>(path_get("/agent")); }

void BluezRoot::register_agent() { std::dynamic_pointer_cast<BluezOrg>(path_get("/org"))->register_agent(_agent); }

std::shared_ptr<SimpleDBus::Proxy> BluezRoot::path_create(const std::string& path) {
    auto child = std::make_shared<BluezOrg>(_conn, _bus_name, path);
    return std::static_pointer_cast<SimpleDBus::Proxy>(child);
}

std::shared_ptr<SimpleDBus::ObjectManager> BluezRoot::object_manager() {
    return std::dynamic_pointer_cast<SimpleDBus::ObjectManager>(interface_get("org.freedesktop.DBus.ObjectManager"));
}
