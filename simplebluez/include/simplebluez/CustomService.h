#pragma once

#include <simpledbus/advanced/Proxy.h>

#include <simplebluez/CustomCharacteristic.h>
#include <simplebluez/interfaces/GattService1.h>

namespace SimpleBluez {

class CustomService : public SimpleDBus::Proxy {
  public:
    CustomService(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomService() = default;

    std::shared_ptr<CustomCharacteristic> create_characteristic();
    void remove_characteristic(const std::string& path);

    // ----- PROPERTIES -----
    std::string uuid();
    void uuid(const std::string& uuid);

    bool primary();
    void primary(bool primary);

  private:

    std::shared_ptr<GattService1> gattservice1();
};

}  // namespace SimpleBluez
