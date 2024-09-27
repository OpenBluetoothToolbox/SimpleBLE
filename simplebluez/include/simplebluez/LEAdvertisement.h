#pragma once

#include <simplebluez/interfaces/LEAdvertisement1.h>
#include <simpledbus/advanced/Proxy.h>

namespace SimpleBluez {

class LEAdvertisement : public SimpleDBus::Proxy {
  public:
    typedef enum {
        DisplayOnly,
        DisplayYesNo,
        KeyboardOnly,
        NoInputNoOutput,
        KeyboardDisplay,
    } Capabilities;

    LEAdvertisement(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~LEAdvertisement() = default;

  private:
    std::shared_ptr<SimpleDBus::Interface> interfaces_create(const std::string& interface_name) override;

    std::shared_ptr<LEAdvertisement1> le_advertisement1();
};

}  // namespace SimpleBluez
