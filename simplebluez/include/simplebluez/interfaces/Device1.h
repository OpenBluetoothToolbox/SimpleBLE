#pragma once

#include <simpledbus/advanced/Interface.h>
#include <simpledbus/advanced/Property.h>
#include <simpledbus/external/kvn_safe_callback.hpp>

#include <string>

namespace SimpleBluez {

class Device1 : public SimpleDBus::Interface {
  public:
    Device1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path);
    virtual ~Device1();

    // ----- METHODS -----
    void Connect();
    void Disconnect();
    void Pair();
    void CancelPairing();

    // ----- PROPERTIES -----
    SimpleDBus::Property<int16_t> RSSI = SimpleDBus::Property<int16_t>(*this, "RSSI");
    SimpleDBus::CachedProperty<int16_t> TxPower = SimpleDBus::CachedProperty<int16_t>(*this, "TxPower");
    SimpleDBus::Property<std::vector<std::string>> UUIDs = SimpleDBus::Property<std::vector<std::string>>(*this, "UUIDs");
    SimpleDBus::CachedProperty<std::map<std::string, std::vector<uint8_t>>> ServiceData = SimpleDBus::CachedProperty<std::map<std::string, std::vector<uint8_t>>>(*this, "ServiceData");
    SimpleDBus::CachedProperty<std::map<uint16_t, std::vector<uint8_t>>> ManufacturerData = SimpleDBus::CachedProperty<std::map<uint16_t, std::vector<uint8_t>>>(*this, "ManufacturerData");

    uint16_t Appearance();  // On Bluez 5.53, this always returns 0.
    std::string Address();
    std::string AddressType();
    std::string Alias();
    std::string Name();
    bool Paired(bool refresh = true);
    bool Connected(bool refresh = true);
    bool ServicesResolved(bool refresh = true);

    // ----- CALLBACKS -----
    kvn::safe_callback<void()> OnServicesResolved;
    kvn::safe_callback<void()> OnDisconnected;

  protected:
    void property_changed(std::string option_name) override;

    std::string _name;
    std::string _alias;
    std::string _address;
    std::string _address_type;
    bool _connected;
    bool _services_resolved;
};

}  // namespace SimpleBluez
