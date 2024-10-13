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

    // ----- METHODS -----
    ByteArray read();
    void write_request(ByteArray value);
    void write_command(ByteArray value);
    void start_notify();
    void stop_notify();

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
    void set_on_value_changed(std::function<void(ByteArray new_value)> callback);
    void clear_on_value_changed();

  private:
    std::shared_ptr<GattCharacteristic1> gattcharacteristic1();
};

}  // namespace SimpleBluez
