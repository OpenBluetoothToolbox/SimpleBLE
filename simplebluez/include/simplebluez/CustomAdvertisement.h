#pragma once

#include <simpledbus/advanced/Proxy.h>

#include <simplebluez/interfaces/LEAdvertisement1.h>
#include <simpledbus/interfaces/ObjectManager.h>

namespace SimpleBluez {

class CustomAdvertisement : public SimpleDBus::Proxy {
  public:

    CustomAdvertisement(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomAdvertisement() = default;

    std::string adv_type();
    void adv_type(const std::string& type);

    std::vector<std::string> service_uuids();
    void service_uuids(const std::vector<std::string>& service_uuids);

    std::map<uint16_t, std::vector<uint8_t>> manufacturer_data();
    void manufacturer_data(const std::map<uint16_t, std::vector<uint8_t>>& manufacturer_data);

    std::map<std::string, std::vector<uint8_t>> service_data();
    void service_data(const std::map<std::string, std::vector<uint8_t>>& service_data);

    std::vector<std::string> solicit_uuids();
    void solicit_uuids(const std::vector<std::string>& solicit_uuids);

    std::map<uint8_t, std::vector<uint8_t>> data();
    void data(const std::map<uint8_t, std::vector<uint8_t>>& data);

    bool discoverable();
    void discoverable(bool discoverable);

    uint16_t discoverable_timeout();
    void discoverable_timeout(uint16_t timeout);

    std::vector<std::string> includes();
    void includes(const std::vector<std::string>& includes);

    std::string local_name();
    void local_name(const std::string& name);

    uint16_t appearance();
    void appearance(uint16_t appearance);

    uint16_t duration();
    void duration(uint16_t duration);

    uint16_t timeout();
    void timeout(uint16_t timeout);

    std::string secondary_channel();
    void secondary_channel(const std::string& channel);

    uint32_t min_interval();
    void min_interval(uint32_t interval);

    uint32_t max_interval();
    void max_interval(uint32_t interval);

    int16_t tx_power();
    void tx_power(int16_t power);

    bool include_tx_power();
    void include_tx_power(bool include);

  private:
    std::shared_ptr<LEAdvertisement1> le_advertisement1();
    std::shared_ptr<SimpleDBus::ObjectManager> object_manager();
};

}  // namespace SimpleBluez
