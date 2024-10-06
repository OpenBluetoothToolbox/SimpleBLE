#pragma once

#include <simpledbus/advanced/Proxy.h>
#include <simpledbus/interfaces/ObjectManager.h>

#include <simplebluez/Adapter.h>
#include <simplebluez/Agent.h>

#include <vector>

namespace SimpleBluez {

class CustomServiceManager : public SimpleDBus::Proxy {
  public:

    CustomServiceManager(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomServiceManager() = default;

  private:
    std::shared_ptr<SimpleDBus::ObjectManager> object_manager();
};

}  // namespace SimpleBluez
