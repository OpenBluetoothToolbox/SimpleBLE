#include "simplebluez/interfaces/GattCharacteristic1.h"
#include "simpledbus/interfaces/Properties.h"
#include "simpledbus/advanced/Proxy.h"

using namespace SimpleBluez;

GattCharacteristic1::GattCharacteristic1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy)
    : SimpleDBus::Interface(conn, proxy, "org.bluez.GattCharacteristic1") {}

GattCharacteristic1::~GattCharacteristic1() { OnValueChanged.unload(); }

void GattCharacteristic1::StartNotify() {
    auto msg = create_method_call("StartNotify");
    _conn->send_with_reply_and_block(msg);
}

void GattCharacteristic1::StopNotify() {
    auto msg = create_method_call("StopNotify");
    _conn->send_with_reply_and_block(msg);
}

void GattCharacteristic1::WriteValue(const ByteArray& value, WriteType type) {
    SimpleDBus::Holder value_data = SimpleDBus::Holder::create_array();
    for (size_t i = 0; i < value.size(); i++) {
        value_data.array_append(SimpleDBus::Holder::create_byte(value[i]));
    }

    SimpleDBus::Holder options = SimpleDBus::Holder::create_dict();
    if (type == WriteType::REQUEST) {
        options.dict_append(SimpleDBus::Holder::Type::STRING, "type", SimpleDBus::Holder::create_string("request"));
    } else if (type == WriteType::COMMAND) {
        options.dict_append(SimpleDBus::Holder::Type::STRING, "type", SimpleDBus::Holder::create_string("command"));
    }

    auto msg = create_method_call("WriteValue");
    msg.append_argument(value_data, "ay");
    msg.append_argument(options, "a{sv}");
    _conn->send_with_reply_and_block(msg);
}

ByteArray GattCharacteristic1::ReadValue() {
    auto msg = create_method_call("ReadValue");

    // NOTE: ReadValue requires an additional argument, which currently is not supported
    SimpleDBus::Holder options = SimpleDBus::Holder::create_dict();
    msg.append_argument(options, "a{sv}");

    SimpleDBus::Message reply_msg = _conn->send_with_reply_and_block(msg);
    SimpleDBus::Holder value = reply_msg.extract();
    update_value(value);

    return Value();
}

std::string GattCharacteristic1::UUID() {
    // As the UUID property doesn't change, we can cache it
    std::scoped_lock lock(_property_update_mutex);
    return _uuid;
}

void GattCharacteristic1::UUID(const std::string& uuid) {
    std::scoped_lock lock(_property_update_mutex);
    _properties["UUID"] = SimpleDBus::Holder::create_string(uuid);
    _uuid = uuid;
}

std::string GattCharacteristic1::Service() {
    std::scoped_lock lock(_property_update_mutex);
    return _properties["Service"].get_string();
}

void GattCharacteristic1::Service(const std::string& service) {
    std::scoped_lock lock(_property_update_mutex);
    _properties["Service"] = SimpleDBus::Holder::create_object_path(service);
}

ByteArray GattCharacteristic1::Value() {
    std::scoped_lock lock(_property_update_mutex);
    return _value;
}

void GattCharacteristic1::Value(const ByteArray& value) {
    SimpleDBus::Holder value_array = SimpleDBus::Holder::create_array();
    for (size_t i = 0; i < value.size(); i++) {
        value_array.array_append(SimpleDBus::Holder::create_byte(value[i]));
    }

    {
        std::scoped_lock lock(_property_update_mutex);
        _properties["Value"] = value_array;
        _value = value;
    }

    std::map<std::string, SimpleDBus::Holder> changed_properties;
    changed_properties["Value"] = value_array;

    std::shared_ptr<SimpleDBus::Properties> properties = std::dynamic_pointer_cast<SimpleDBus::Properties>(_proxy->interface_get("org.freedesktop.DBus.Properties"));
    properties->PropertiesChanged("org.bluez.GattCharacteristic1", changed_properties);
}

std::vector<std::string> GattCharacteristic1::Flags() {
    std::scoped_lock lock(_property_update_mutex);

    std::vector<std::string> flags;
    for (SimpleDBus::Holder& flag : _properties["Flags"].get_array()) {
        flags.push_back(flag.get_string());
    }

    return flags;
}

void GattCharacteristic1::Flags(const std::vector<std::string>& flags) {
    std::scoped_lock lock(_property_update_mutex);
    SimpleDBus::Holder flags_array = SimpleDBus::Holder::create_array();
    for (const std::string& flag : flags) {
        flags_array.array_append(SimpleDBus::Holder::create_string(flag));
    }
    _properties["Flags"] = flags_array;
}

uint16_t GattCharacteristic1::MTU() {
    std::scoped_lock lock(_property_update_mutex);
    return _properties["MTU"].get_uint16();
}

void GattCharacteristic1::MTU(uint16_t mtu) {
    std::scoped_lock lock(_property_update_mutex);
    _properties["MTU"] = SimpleDBus::Holder::create_uint16(mtu);
}

bool GattCharacteristic1::Notifying(bool refresh) {
    if (refresh) {
        property_refresh("Notifying");
    }

    std::scoped_lock lock(_property_update_mutex);
    return _properties["Notifying"].get_boolean();
}

void GattCharacteristic1::property_changed(std::string option_name) {
    if (option_name == "UUID") {
        std::scoped_lock lock(_property_update_mutex);
        _uuid = _properties["UUID"].get_string();
    } else if (option_name == "Value") {
        update_value(_properties["Value"]);
        OnValueChanged();
    }
}

void GattCharacteristic1::update_value(SimpleDBus::Holder& new_value) {
    std::scoped_lock lock(_property_update_mutex);
    auto value_array = new_value.get_array();

    char* value_data = new char[value_array.size()];
    for (std::size_t i = 0; i < value_array.size(); i++) {
        value_data[i] = value_array[i].get_byte();
    }
    _value = ByteArray(value_data, value_array.size());
    delete[] value_data;
}

void GattCharacteristic1::message_handle(SimpleDBus::Message& msg) {
    if (msg.is_method_call(_interface_name, "ReadValue")) {
        SimpleDBus::Holder options = msg.extract();

        OnReadValue();

        SimpleDBus::Message reply = SimpleDBus::Message::create_method_return(msg);
        reply.append_argument(_properties["Value"], "ay");
        _conn->send(reply);
    } else if (msg.is_method_call(_interface_name, "WriteValue")) {
        SimpleDBus::Holder value = msg.extract();
        msg.extract_next();
        SimpleDBus::Holder options = msg.extract();

        update_value(value);
        SimpleDBus::Message reply = SimpleDBus::Message::create_method_return(msg);
        _conn->send(reply);

        OnWriteValue(_value);
    } else if (msg.is_method_call(_interface_name, "StartNotify")) {
        SimpleDBus::Message reply = SimpleDBus::Message::create_method_return(msg);
        _conn->send(reply);

        {
            std::scoped_lock lock(_property_update_mutex);
            _properties["Notifying"] = SimpleDBus::Holder::create_boolean(true);

            std::map<std::string, SimpleDBus::Holder> changed_properties;
            changed_properties["Notifying"] = SimpleDBus::Holder::create_boolean(true);

            std::shared_ptr<SimpleDBus::Properties> properties = std::dynamic_pointer_cast<SimpleDBus::Properties>(_proxy->interface_get("org.freedesktop.DBus.Properties"));
            properties->PropertiesChanged("org.bluez.GattCharacteristic1", changed_properties);
        }

        OnStartNotify();
    } else if (msg.is_method_call(_interface_name, "StopNotify")) {
        SimpleDBus::Message reply = SimpleDBus::Message::create_method_return(msg);
        _conn->send(reply);

        {
            std::scoped_lock lock(_property_update_mutex);
            _properties["Notifying"] = SimpleDBus::Holder::create_boolean(false);

            std::map<std::string, SimpleDBus::Holder> changed_properties;
            changed_properties["Notifying"] = SimpleDBus::Holder::create_boolean(false);

            std::shared_ptr<SimpleDBus::Properties> properties = std::dynamic_pointer_cast<SimpleDBus::Properties>(_proxy->interface_get("org.freedesktop.DBus.Properties"));
            properties->PropertiesChanged("org.bluez.GattCharacteristic1", changed_properties);
        }

        OnStopNotify();
    }
}
