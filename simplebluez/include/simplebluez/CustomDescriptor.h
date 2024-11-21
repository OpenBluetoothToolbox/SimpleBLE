#pragma once

#include <simpledbus/advanced/Proxy.h>

#include <simplebluez/Types.h>
#include <simplebluez/interfaces/GattDescriptor1.h>

#include <cstdlib>

namespace SimpleBluez {

class CustomDescriptor : public SimpleDBus::Proxy {
  public:
    CustomDescriptor(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomDescriptor();

    // ----- METHODS -----
    ByteArray read();
    void write(ByteArray value);

    // ----- PROPERTIES -----
    std::string uuid();
    ByteArray value();

    // ----- CALLBACKS -----
    void set_on_value_changed(std::function<void(ByteArray new_value)> callback);
    void clear_on_value_changed();

  private:
    std::shared_ptr<GattDescriptor1> gattdescriptor1();
};

}  // namespace SimpleBluez
