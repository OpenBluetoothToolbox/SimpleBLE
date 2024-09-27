#pragma once

#include <simpledbus/advanced/Interface.h>

#include <optional>
#include <string>
#include <vector>

namespace SimpleBluez {

class LEAdvertisement1 : public SimpleDBus::Interface {
  public:
    // ----- TYPES -----


    // ----- CONSTRUCTORS -----
    LEAdvertisement1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path);
    virtual ~LEAdvertisement1() = default;

    // ----- PROPERTIES -----
    void SetType(const std::string& type);
    void SetServiceUUIDs(const std::vector<std::string>& uuids);
    void SetManufacturerData(const std::map<uint16_t, std::vector<uint8_t>>& data);
    void SetServiceData(const std::map<std::string, std::vector<uint8_t>>& data);
    void SetIncludeTxPower(bool include);

    // ----- METHODS -----
    void Release();

  protected:
    void property_changed(std::string option_name) override;
    void message_handle(SimpleDBus::Message& msg) override;
};

}  // namespace SimpleBluez