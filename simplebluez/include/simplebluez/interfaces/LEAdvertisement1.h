#pragma once
#include <simpledbus/advanced/Interface.h>
#include <optional>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace SimpleBluez {

class LEAdvertisement1 : public SimpleDBus::Interface {
  public:
    // ----- CONSTRUCTORS -----

    LEAdvertisement1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy);
    virtual ~LEAdvertisement1() = default;

    // ----- PROPERTIES -----

    void SetType(const std::string& type);
    void SetServiceUUIDs(const std::vector<std::string>& uuids);
    void SetManufacturerData(const std::map<uint16_t, std::vector<uint8_t>>& data);
    void SetServiceData(const std::map<std::string, std::vector<uint8_t>>& data);
    void SetSolicitUUIDs(const std::vector<std::string>& uuids);
    void SetData(const std::map<uint8_t, std::vector<uint8_t>>& data);
    void SetDiscoverable(bool discoverable);
    void SetDiscoverableTimeout(uint16_t timeout);
    void SetIncludes(const std::vector<std::string>& includes);
    void SetLocalName(const std::string& name);
    void SetAppearance(uint16_t appearance);
    void SetDuration(uint16_t duration);
    void SetTimeout(uint16_t timeout);
    void SetSecondaryChannel(const std::string& channel);
    void SetMinInterval(uint32_t interval);
    void SetMaxInterval(uint32_t interval);
    void SetTxPower(int16_t power);
    void SetIncludeTxPower(bool include);

    // ----- METHODS -----

    void Release();

  protected:
    void message_handle(SimpleDBus::Message& msg) override;

  private:
};

}  // namespace SimpleBluez