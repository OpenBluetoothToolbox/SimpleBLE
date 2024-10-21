#pragma once
#include <simpledbus/advanced/Interface.h>
#include <optional>
#include <string>
#include <vector>

namespace SimpleBluez {

class GattManager1 : public SimpleDBus::Interface {

  public:

    // ----- TYPES -----

    // ----- CONSTRUCTORS -----

    GattManager1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy);
    virtual ~GattManager1() = default;

    // ----- METHODS -----

    void RegisterApplication(std::string application_path);
    void UnregisterApplication(std::string application_path);

    // ----- PROPERTIES -----

  protected:
  
};

}  // namespace SimpleBluez