#pragma once

#include <simpledbus/advanced/Interface.h>
#include <simpledbus/external/kvn_safe_callback.hpp>

#include <simplebluez/Types.h>

#include <string>

namespace SimpleBluez {

class GattCharacteristic1 : public SimpleDBus::Interface {
  public:
    typedef enum { REQUEST = 0, COMMAND } WriteType;

    GattCharacteristic1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy);
    virtual ~GattCharacteristic1();

    // ----- METHODS -----
    void StartNotify();
    void StopNotify();
    void WriteValue(const ByteArray& value, WriteType type);
    ByteArray ReadValue();

    // ----- PROPERTIES -----
    std::string UUID();
    void UUID(const std::string& uuid);

    std::string Service();
    void Service(const std::string& service);

    ByteArray Value();
    void Value(const ByteArray& value);

    bool Notifying(bool refresh = true);

    std::vector<std::string> Flags();
    void Flags(const std::vector<std::string>& flags);

    uint16_t MTU();
    void MTU(uint16_t mtu);

    // ----- CALLBACKS -----
    kvn::safe_callback<void()> OnValueChanged;
    kvn::safe_callback<void(ByteArray value)> OnWriteValue;
    kvn::safe_callback<void()> OnReadValue;
    kvn::safe_callback<void()> OnStartNotify;
    kvn::safe_callback<void()> OnStopNotify;

    void message_handle(SimpleDBus::Message& msg) override;

  protected:
    void property_changed(std::string option_name) override;
    void update_value(SimpleDBus::Holder& new_value);

    std::string _uuid;
    ByteArray _value;
};

}  // namespace SimpleBluez
