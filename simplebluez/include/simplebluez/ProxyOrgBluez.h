#pragma once

#include <simpledbus/advanced/RemoteProxy.h>

#include <simplebluez/Adapter.h>
#include <simplebluez/Agent.h>

#include <simplebluez/interfaces/AgentManager1.h>

namespace SimpleBluez {

class ProxyOrgBluez : public SimpleDBus::RemoteProxy {
  public:
    ProxyOrgBluez(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~ProxyOrgBluez() = default;

    void register_agent(std::shared_ptr<Agent> agent);

    std::vector<std::shared_ptr<Adapter>> get_adapters();

  private:
    std::shared_ptr<SimpleDBus::RemoteProxy> path_create(const std::string& path) override;
    std::shared_ptr<SimpleDBus::RemoteInterface> interfaces_create(const std::string& interface_name) override;
    std::shared_ptr<AgentManager1> agentmanager1();
};

}  // namespace SimpleBluez
