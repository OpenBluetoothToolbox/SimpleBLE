#pragma once

#include <simpledbus/advanced/Proxy.h>

#include <simplebluez/CustomCharacteristic.h>
#include <simplebluez/interfaces/GattService1.h>

namespace SimpleBluez {

class CustomService : public SimpleDBus::Proxy {
  public:
    CustomService(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomService() = default;

    std::shared_ptr<CustomCharacteristic> get_characteristic(const std::string& uuid);

    // ----- PROPERTIES -----
    std::vector<std::shared_ptr<CustomCharacteristic>> characteristics();

    std::string uuid();

  private:
    std::shared_ptr<SimpleDBus::Proxy> path_create(const std::string& path) override;
    std::shared_ptr<SimpleDBus::Interface> interfaces_create(const std::string& interface_name) override;

    std::shared_ptr<GattService1> gattservice1();
};

}  // namespace SimpleBluez
