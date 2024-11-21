#pragma once

#include <simpledbus/advanced/Proxy.h>

#include <simplebluez/Device.h>
#include <simplebluez/CustomAdvertisement.h>
#include <simplebluez/interfaces/Adapter1.h>
#include <simplebluez/interfaces/LEAdvertisingManager1.h>
#include <simplebluez/interfaces/GattManager1.h>

#include <functional>

namespace SimpleBluez {

class Adapter : public SimpleDBus::Proxy {
  public:
    typedef Adapter1::DiscoveryFilter DiscoveryFilter;

    Adapter(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~Adapter();

    std::string identifier() const;
    std::string address();
    std::string alias();
    void alias(std::string alias);
    bool discovering();
    bool powered();
    void powered(bool powered);

    void discovery_filter(const DiscoveryFilter& filter);
    void discovery_start();
    void discovery_stop();

    std::shared_ptr<Device> device_get(const std::string& path);
    void device_remove(const std::string& path);
    void device_remove(const std::shared_ptr<Device>& device);

    std::vector<std::shared_ptr<Device>> device_paired_get();
    std::vector<std::shared_ptr<Device>> device_bonded_get();

    void set_on_device_updated(std::function<void(std::shared_ptr<Device> device)> callback);
    void clear_on_device_updated();

    void register_advertisement(const std::shared_ptr<CustomAdvertisement>& advertisement);
    void unregister_advertisement(const std::shared_ptr<CustomAdvertisement>& advertisement);

    uint8_t active_advertisement_instances(bool refresh = true);
    uint8_t supported_advertisement_instances(bool refresh = true);

    void register_application(const std::string& application_path);
    void unregister_application(const std::string& application_path);

  private:
    std::shared_ptr<SimpleDBus::Proxy> path_create(const std::string& path) override;
    std::shared_ptr<SimpleDBus::Interface> interfaces_create(const std::string& interface_name) override;

    std::shared_ptr<Adapter1> adapter1();
    std::shared_ptr<LEAdvertisingManager1> le_advertising_manager1();
    std::shared_ptr<GattManager1> gatt_manager1();

    kvn::safe_callback<void(std::shared_ptr<Device> device)> _on_device_updated;
};

}  // namespace SimpleBluez
