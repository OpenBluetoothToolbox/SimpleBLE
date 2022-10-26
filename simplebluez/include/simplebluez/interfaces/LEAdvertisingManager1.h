#pragma once

#include <simpledbus/advanced/Interface.h>

#include <optional>
#include <string>
#include <vector>

namespace SimpleBluez {

class LEAdvertisingManager1 : public SimpleDBus::Interface {
  public:
    // ----- TYPES -----


    // ----- CONSTRUCTORS -----
    LEAdvertisingManager1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path);
    virtual ~LEAdvertisingManager1() = default;

    // ----- METHODS -----
    void RegisterAdvertisement(std::string advertisement_path);
    void UnregisterAdvertisement(std::string advertisement_path);


    // ----- PROPERTIES -----
    uint8_t ActiveInstances(bool refresh = true);
    uint8_t SupportedInstances(bool refresh = true);
    std::vector<std::string> SupportedIncludes(bool refresh = true);

  protected:
    void property_changed(std::string option_name) override;
};

}  // namespace SimpleBluez