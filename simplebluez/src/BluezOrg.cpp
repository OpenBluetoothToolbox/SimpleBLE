#include <simplebluez/BluezOrg.h>
#include <simplebluez/BluezOrgBluez.h>

using namespace SimpleBluez;

BluezOrg::BluezOrg(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {}

std::vector<std::shared_ptr<Adapter>> BluezOrg::get_adapters() {
    return std::dynamic_pointer_cast<BluezOrgBluez>(path_get("/org/bluez"))->get_adapters();
}

void BluezOrg::register_agent(std::shared_ptr<Agent> agent) {
    std::dynamic_pointer_cast<BluezOrgBluez>(path_get("/org/bluez"))->register_agent(agent);
}

std::shared_ptr<SimpleDBus::Proxy> BluezOrg::path_create(const std::string& path) {
    auto child = std::make_shared<BluezOrgBluez>(_conn, _bus_name, path);
    return std::static_pointer_cast<SimpleDBus::Proxy>(child);
}

std::shared_ptr<SimpleDBus::Interface> BluezOrg::interfaces_create(const std::string& interface_name) {
    return std::make_shared<SimpleDBus::Interface>(_conn, this, interface_name);
}
