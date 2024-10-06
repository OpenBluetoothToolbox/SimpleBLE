#pragma once

#include <simpledbus/advanced/Proxy.h>
#include <simpledbus/interfaces/ObjectManager.h>

#include <simplebluez/CustomAdvertisementManager.h>
#include <simplebluez/CustomServiceManager.h>
#include <vector>

namespace SimpleBluez {

class CustomRoot : public SimpleDBus::Proxy {
  public:

    CustomRoot(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomRoot() = default;

    std::shared_ptr<CustomAdvertisementManager> get_custom_advertisements();
    std::shared_ptr<CustomServiceManager> get_custom_services();

  private:
    std::shared_ptr<SimpleDBus::ObjectManager> object_manager();

    std::shared_ptr<CustomAdvertisementManager> _custom_advertisements;
    std::shared_ptr<CustomServiceManager> _custom_services;
};

}  // namespace SimpleBluez
