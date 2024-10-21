#include <simplebluez/Adapter.h>
#include <simplebluez/BluezOrgBluez.h>

using namespace SimpleBluez;

BluezOrgBluez::BluezOrgBluez(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name,
                             const std::string& path)
    : Proxy(conn, bus_name, path) {}

std::shared_ptr<SimpleDBus::Proxy> BluezOrgBluez::path_create(const std::string& path) {
    auto child = std::make_shared<Adapter>(_conn, _bus_name, path);
    return std::static_pointer_cast<SimpleDBus::Proxy>(child);
}

std::shared_ptr<SimpleDBus::Interface> BluezOrgBluez::interfaces_create(const std::string& interface_name) {
    if (interface_name == "org.bluez.AgentManager1") {
        return std::static_pointer_cast<SimpleDBus::Interface>(std::make_shared<AgentManager1>(_conn, this));
    }

    return std::make_shared<SimpleDBus::Interface>(_conn, this, interface_name);
}

std::shared_ptr<AgentManager1> BluezOrgBluez::agentmanager1() {
    return std::dynamic_pointer_cast<AgentManager1>(interface_get("org.bluez.AgentManager1"));
}

void BluezOrgBluez::register_agent(std::shared_ptr<Agent> agent) {
    agentmanager1()->RegisterAgent(agent->path(), agent->capabilities());
}

std::vector<std::shared_ptr<Adapter>> BluezOrgBluez::get_adapters() { return children_casted<Adapter>(); }
