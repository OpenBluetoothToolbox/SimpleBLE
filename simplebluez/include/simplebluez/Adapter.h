#pragma once

#include <simpledbus/advanced/Proxy.h>

#include <simplebluez/Device.h>
#include <simplebluez/interfaces/Adapter1.h>

#include <functional>

namespace SimpleBluez {

class Adapter : public SimpleDBus::Proxy {
  public:
    typedef Adapter1::DiscoveryFilter DiscoveryFilter;

    Adapter(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~Adapter();

    std::string identifier() const;
    std::string address();
    std::string power_state();
    bool discovering();
    bool powered();

    void discovery_filter(const DiscoveryFilter& filter);
    void discovery_start();
    void discovery_stop();

    std::shared_ptr<Device> device_get(const std::string& path);
    void device_remove(const std::string& path);
    void device_remove(const std::shared_ptr<Device>& device);
    std::vector<std::shared_ptr<Device>> device_paired_get();

    void set_on_device_updated(std::function<void(std::shared_ptr<Device> device)> callback);
    void clear_on_device_updated();

    void set_on_power_state_changed(std::function<void(const std::string& state)> callback);
    void clear_on_power_state_changed();

  private:
    std::shared_ptr<SimpleDBus::Proxy> path_create(const std::string& path) override;
    std::shared_ptr<SimpleDBus::Interface> interfaces_create(const std::string& interface_name) override;

    std::shared_ptr<Adapter1> adapter1();
};

}  // namespace SimpleBluez
