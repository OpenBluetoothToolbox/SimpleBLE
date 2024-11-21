#pragma once

#include <simpledbus/advanced/Proxy.h>

#include <simplebluez/CustomDescriptor.h>
#include <simplebluez/Types.h>
#include <simplebluez/interfaces/GattCharacteristic1.h>

#include <cstdlib>

namespace SimpleBluez {

class CustomCharacteristic : public SimpleDBus::Proxy {
  public:
    CustomCharacteristic(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomCharacteristic();

    std::shared_ptr<CustomDescriptor> create_descriptor();
    void remove_descriptor(const std::string& path);

    // ----- PROPERTIES -----
    std::vector<std::shared_ptr<CustomDescriptor>> descriptors();

    std::string uuid();
    void uuid(const std::string& uuid);

    std::string service();
    void service(const std::string& service);

    ByteArray value();
    void value(const ByteArray& value);

    bool notifying();

    std::vector<std::string> flags();
    void flags(const std::vector<std::string>& flags);
    uint16_t mtu();
    void mtu(uint16_t mtu);

    // ----- CALLBACKS -----
    void set_on_read_value(std::function<void()> callback);
    void clear_on_read_value();

    void set_on_write_value(std::function<void(ByteArray value)> callback);
    void clear_on_write_value();

    void set_on_notify(std::function<void(bool)> callback);
    void clear_on_notify();

  private:
    std::shared_ptr<GattCharacteristic1> gattcharacteristic1();
};

}  // namespace SimpleBluez
