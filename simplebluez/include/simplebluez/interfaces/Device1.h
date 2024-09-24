#pragma once

#include <simpledbus/advanced/Interface.h>
#include <simpledbus/advanced/Property.h>
#include <simpledbus/external/kvn_safe_callback.hpp>

#include <string>

#include "simplebluez/Types.h"

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
    SimpleDBus::Property<int16_t> RSSI = create_property<int16_t>("RSSI");
    SimpleDBus::CachedProperty<int16_t> TxPower = create_cached_property<int16_t>("TxPower");
    SimpleDBus::Property<std::vector<std::string>> UUIDs = create_property<std::vector<std::string>>("UUIDs");
    SimpleDBus::CachedProperty<std::map<std::string, std::vector<uint8_t>>> ServiceData = create_cached_property<std::map<std::string, std::vector<uint8_t>>>("ServiceData");
    SimpleDBus::CachedProperty<std::map<uint16_t, std::vector<uint8_t>>> ManufacturerData = create_cached_property<std::map<uint16_t, std::vector<uint8_t>>>("ManufacturerData");

    SimpleDBus::Property<uint16_t> Appearance = create_property<uint16_t>("Appearance");
    SimpleDBus::Property<std::string> Address = create_property<std::string>("Address");
    SimpleDBus::Property<std::string> AddressType = create_property<std::string>("AddressType");
    SimpleDBus::Property<std::string> Alias = create_property<std::string>("Alias");
    SimpleDBus::Property<std::string> Name = create_property<std::string>("Name");
    SimpleDBus::Property<bool> Paired = create_property<bool>("Paired");
    SimpleDBus::Property<bool> Connected = create_property<bool>("Connected");
    SimpleDBus::Property<bool> ServicesResolved = create_property<bool>("ServicesResolved");

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
