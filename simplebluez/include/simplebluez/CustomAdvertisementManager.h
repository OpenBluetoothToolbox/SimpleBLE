#pragma once

#include <simpledbus/advanced/Proxy.h>
#include <simpledbus/interfaces/ObjectManager.h>

#include <simplebluez/CustomAdvertisement.h>

#include <vector>

namespace SimpleBluez {

class CustomAdvertisementManager : public SimpleDBus::Proxy {
  public:

    CustomAdvertisementManager(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomAdvertisementManager() = default;

    std::shared_ptr<SimpleBluez::CustomAdvertisement> create_advertisement(const std::string& path);
    void remove_advertisement(const std::string& path);

  private:
    std::shared_ptr<SimpleDBus::ObjectManager> object_manager();
};

}  // namespace SimpleBluez
