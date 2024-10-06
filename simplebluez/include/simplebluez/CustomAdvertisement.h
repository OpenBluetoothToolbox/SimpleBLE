#pragma once

#include <simpledbus/advanced/Proxy.h>
#include <simplebluez/interfaces/LEAdvertisement1.h>

namespace SimpleBluez {

class CustomAdvertisement : public SimpleDBus::Proxy {
  public:

    CustomAdvertisement(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomAdvertisement() = default;

  private:
    std::shared_ptr<LEAdvertisement1> le_advertisement1();
};

}  // namespace SimpleBluez
