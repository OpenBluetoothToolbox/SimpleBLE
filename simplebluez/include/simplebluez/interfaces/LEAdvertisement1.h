#pragma once
#include <simpledbus/advanced/Interface.h>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace SimpleBluez {

class LEAdvertisement1 : public SimpleDBus::Interface {
  public:
    // ----- CONSTRUCTORS -----

    LEAdvertisement1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy);
    virtual ~LEAdvertisement1() = default;

    // ----- PROPERTIES -----

    std::string Type();
    void Type(const std::string& type);

    std::vector<std::string> ServiceUUIDs();
    void ServiceUUIDs(const std::vector<std::string>& uuids);

    std::map<uint16_t, std::vector<uint8_t>> ManufacturerData();
    void ManufacturerData(const std::map<uint16_t, std::vector<uint8_t>>& data);

    std::map<std::string, std::vector<uint8_t>> ServiceData();
    void ServiceData(const std::map<std::string, std::vector<uint8_t>>& data);

    std::vector<std::string> SolicitUUIDs();
    void SolicitUUIDs(const std::vector<std::string>& uuids);

    std::map<uint8_t, std::vector<uint8_t>> Data();
    void Data(const std::map<uint8_t, std::vector<uint8_t>>& data);

    bool Discoverable();
    void Discoverable(bool discoverable);

    uint16_t DiscoverableTimeout();
    void DiscoverableTimeout(uint16_t timeout);

    std::vector<std::string> Includes();
    void Includes(const std::vector<std::string>& includes);

    std::string LocalName();
    void LocalName(const std::string& name);

    uint16_t Appearance();
    void Appearance(uint16_t appearance);

    uint16_t Duration();
    void Duration(uint16_t duration);

    uint16_t Timeout();
    void Timeout(uint16_t timeout);

    std::string SecondaryChannel();
    void SecondaryChannel(const std::string& channel);

    uint32_t MinInterval();
    void MinInterval(uint32_t interval);

    uint32_t MaxInterval();
    void MaxInterval(uint32_t interval);

    int16_t TxPower();
    void TxPower(int16_t power);

    bool IncludeTxPower();
    void IncludeTxPower(bool include);
    // ----- METHODS -----

    void Release();

  protected:
  private:
};

}  // namespace SimpleBluez