#pragma once

#include <simpledbus/advanced/Interface.h>

#include <string>

namespace SimpleBluez {

class GattService1 : public SimpleDBus::Interface {
  public:
    GattService1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy);
    virtual ~GattService1() = default;

    // ----- METHODS -----

    // ----- PROPERTIES -----
    std::string UUID();
    void UUID(const std::string& uuid);

    bool Primary();
    void Primary(bool primary);

    std::vector<std::string> Characteristics();
    void Characteristics(const std::vector<std::string>& characteristics);

  protected:
    void property_changed(std::string option_name) override;

    std::string _uuid;
};

}  // namespace SimpleBluez
