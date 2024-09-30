#include <simplebluez/Bluez.h>
#include <simplebluez/ProxyOrg.h>
#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleBluez;

#ifdef SIMPLEBLUEZ_USE_SESSION_DBUS
#define DBUS_BUS DBUS_BUS_SESSION
#else
#define DBUS_BUS DBUS_BUS_SYSTEM
#endif

Bluez::Bluez() : Proxy(std::make_shared<SimpleDBus::Connection>(DBUS_BUS), "org.bluez", "/") {
}

Bluez::~Bluez() {
    if (_conn->is_initialized()) {
        _conn->remove_match("type='signal',sender='org.bluez'");
    }
}

void Bluez::init() {
    _conn->init();

    // FIXME: This a hack that we have to do to ensure that the root object path is registered. The underlying
    // problem is that the Proxy object owns the Connection object and can't register while the constructor is running.
    // The proper solution is is to split the Bluez class into a ProxyRoot object that doesn't have a connection and
    // some sort of controller object that owns the connection separately.
    register_object_path();

    const std::shared_ptr<SimpleDBus::Proxy> proxy = std::static_pointer_cast<SimpleDBus::Proxy>(shared_from_this());

    _interfaces["org.freedesktop.DBus.ObjectManager"] = std::static_pointer_cast<SimpleDBus::Interface>(
        std::make_shared<SimpleDBus::ObjectManager>(_conn, proxy));

    object_manager()->InterfacesAdded = [&](std::string path, SimpleDBus::Holder options) { path_add(path, options); };
    object_manager()->InterfacesRemoved = [&](std::string path, SimpleDBus::Holder options) {
        path_remove(path, options);
    };

    // Load all managed objects
    SimpleDBus::Holder managed_objects = object_manager()->GetManagedObjects();
    for (auto& [path, managed_interfaces] : managed_objects.get_dict_object_path()) {
        path_add(path, managed_interfaces);
    }

    _conn->add_match("type='signal',sender='org.bluez'");

    // Create the agent that will handle pairing.
    _agent = std::make_shared<Agent>(_conn, "org.bluez", "/agent");
    _agent->init();
    path_append_child("/agent", std::static_pointer_cast<SimpleDBus::Proxy>(_agent));
}

void Bluez::run_async() {
    _conn->read_write_dispatch();
}

std::vector<std::shared_ptr<Adapter>> Bluez::get_adapters() {
    return std::dynamic_pointer_cast<ProxyOrg>(path_get("/org"))->get_adapters();
}

std::shared_ptr<Agent> Bluez::get_agent() { return std::dynamic_pointer_cast<Agent>(path_get("/agent")); }

void Bluez::register_agent() { std::dynamic_pointer_cast<ProxyOrg>(path_get("/org"))->register_agent(_agent); }

std::shared_ptr<SimpleDBus::Proxy> Bluez::path_create(const std::string& path) {
    auto child = std::make_shared<ProxyOrg>(_conn, _bus_name, path);
    return std::static_pointer_cast<SimpleDBus::Proxy>(child);
}

std::shared_ptr<SimpleDBus::ObjectManager> Bluez::object_manager() {
    return std::dynamic_pointer_cast<SimpleDBus::ObjectManager>(interface_get("org.freedesktop.DBus.ObjectManager"));
}
