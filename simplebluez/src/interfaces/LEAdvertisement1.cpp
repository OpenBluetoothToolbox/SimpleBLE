#include "simplebluez/interfaces/LEAdvertisement1.h"

using namespace SimpleBluez;

LEAdvertisement1::LEAdvertisement1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path)
    : SimpleDBus::LocalInterface(conn, "org.bluez", path, "org.bluez.LEAdvertisement1") {

    // Set all fields to null or empty values
    SetType("peripheral"); // This is the only required field, so we set a default
    // SetServiceUUIDs({}); // Empty vector
    // SetManufacturerData({}); // Empty map
    // SetServiceData({}); // Empty map
    // SetSolicitUUIDs({}); // Empty vector
    // SetData({}); // Empty map
    // SetDiscoverable(false);
    // SetDiscoverableTimeout(0);
    // SetIncludes({}); // Empty vector
    SetLocalName("SimpleBluez");
    // SetAppearance(0);
    // SetDuration(180);
    SetTimeout(180);
    // SetSecondaryChannel("");
    // SetMinInterval(0);
    // SetMaxInterval(0);
    // SetTxPower(0);
    // SetIncludeTxPower(false);
}

void LEAdvertisement1::SetType(const std::string& type) {
    _properties["Type"] = SimpleDBus::Holder::create_string(type);
}

void LEAdvertisement1::SetServiceUUIDs(const std::vector<std::string>& uuids) {
    SimpleDBus::Holder uuids_holder = SimpleDBus::Holder::create_array();
    for (const auto& uuid : uuids) {
        uuids_holder.array_append(SimpleDBus::Holder::create_string(uuid));
    }
    _properties["ServiceUUIDs"] = uuids_holder;
}

void LEAdvertisement1::SetManufacturerData(const std::map<uint16_t, std::vector<uint8_t>>& data) {
    SimpleDBus::Holder data_holder = SimpleDBus::Holder::create_dict();
    for (const auto& [key, value] : data) {
        SimpleDBus::Holder value_holder = SimpleDBus::Holder::create_array();
        for (uint8_t byte : value) {
            value_holder.array_append(SimpleDBus::Holder::create_byte(byte));
        }
        data_holder.dict_append(SimpleDBus::Holder::Type::UINT16, key, value_holder);
    }
    _properties["ManufacturerData"] = data_holder;
}

void LEAdvertisement1::SetServiceData(const std::map<std::string, std::vector<uint8_t>>& data) {
    SimpleDBus::Holder data_holder = SimpleDBus::Holder::create_dict();
    for (const auto& [key, value] : data) {
        SimpleDBus::Holder value_holder = SimpleDBus::Holder::create_array();
        for (uint8_t byte : value) {
            value_holder.array_append(SimpleDBus::Holder::create_byte(byte));
        }
        data_holder.dict_append(SimpleDBus::Holder::Type::STRING, key, value_holder);
    }
    _properties["ServiceData"] = data_holder;
}

void LEAdvertisement1::SetIncludeTxPower(bool include) {
    _properties["IncludeTxPower"] = SimpleDBus::Holder::create_boolean(include);
}

void LEAdvertisement1::Release() {
    // This method is called when the advertisement is unregistered
    // Implement any cleanup logic here
}

#include <iostream>

void LEAdvertisement1::message_handle(SimpleDBus::Message& msg) {
    std::cout << "LEAdvertisement1::message_handle: " << msg.to_string() << std::endl;
    // if (msg.get_type() == SimpleDBus::Message::Type::METHOD_CALL) {
    //     SimpleDBus::Message reply = SimpleDBus::Message::create_method_return(msg);

    //     if (msg.get_member() == "GetAll" && msg.get_interface() == "org.freedesktop.DBus.Properties") {
    //         SimpleDBus::Holder properties = SimpleDBus::Holder::create_dict();
    //         for (const auto& [key, value] : _properties) {
    //             properties.dict_append(SimpleDBus::Holder::Type::STRING, key, value);
    //         }
    //         reply.append_argument(properties, "a{sv}");
    //     } else if (msg.get_member() == "Get" && msg.get_interface() == "org.freedesktop.DBus.Properties") {
    //         std::string interface_name = msg.extract().get_string();
    //         std::string property_name = msg.extract().get_string();
    //         if (_properties.find(property_name) != _properties.end()) {
    //             reply.append_argument(_properties[property_name], "v");
    //         } else {
    //             // Property not found, create an error reply
    //             reply = SimpleDBus::Message::create_error(msg, "org.freedesktop.DBus.Error.UnknownProperty", "Unknown property: " + property_name);
    //         }
    //     } else if (msg.get_member() == "Release") {
    //         Release();
    //     } else {
    //         // Unknown method, create an error reply
    //         reply = SimpleDBus::Message::create_error(msg, "org.freedesktop.DBus.Error.UnknownMethod", "Unknown method: " + msg.get_member());
    //     }

    //     _conn->send(reply);
    // }
}

void LEAdvertisement1::SetSolicitUUIDs(const std::vector<std::string>& uuids) {
    SimpleDBus::Holder uuids_holder = SimpleDBus::Holder::create_array();
    for (const auto& uuid : uuids) {
        uuids_holder.array_append(SimpleDBus::Holder::create_string(uuid));
    }
    _properties["SolicitUUIDs"] = uuids_holder;
}

void LEAdvertisement1::SetData(const std::map<uint8_t, std::vector<uint8_t>>& data) {
    SimpleDBus::Holder data_holder = SimpleDBus::Holder::create_dict();
    for (const auto& [key, value] : data) {
        SimpleDBus::Holder value_holder = SimpleDBus::Holder::create_array();
        for (uint8_t byte : value) {
            value_holder.array_append(SimpleDBus::Holder::create_byte(byte));
        }
        data_holder.dict_append(SimpleDBus::Holder::Type::BYTE, key, value_holder);
    }
    _properties["Data"] = data_holder;
}

void LEAdvertisement1::SetDiscoverable(bool discoverable) {
    _properties["Discoverable"] = SimpleDBus::Holder::create_boolean(discoverable);
}

void LEAdvertisement1::SetDiscoverableTimeout(uint16_t timeout) {
    _properties["DiscoverableTimeout"] = SimpleDBus::Holder::create_uint16(timeout);
}

void LEAdvertisement1::SetIncludes(const std::vector<std::string>& includes) {
    SimpleDBus::Holder includes_holder = SimpleDBus::Holder::create_array();
    for (const auto& include : includes) {
        includes_holder.array_append(SimpleDBus::Holder::create_string(include));
    }
    _properties["Includes"] = includes_holder;
}

void LEAdvertisement1::SetLocalName(const std::string& name) {
    _properties["LocalName"] = SimpleDBus::Holder::create_string(name);
}

void LEAdvertisement1::SetAppearance(uint16_t appearance) {
    _properties["Appearance"] = SimpleDBus::Holder::create_uint16(appearance);
}

void LEAdvertisement1::SetDuration(uint16_t duration) {
    _properties["Duration"] = SimpleDBus::Holder::create_uint16(duration);
}

void LEAdvertisement1::SetTimeout(uint16_t timeout) {
    _properties["Timeout"] = SimpleDBus::Holder::create_uint16(timeout);
}

void LEAdvertisement1::SetSecondaryChannel(const std::string& channel) {
    _properties["SecondaryChannel"] = SimpleDBus::Holder::create_string(channel);
}

void LEAdvertisement1::SetMinInterval(uint32_t interval) {
    _properties["MinInterval"] = SimpleDBus::Holder::create_uint32(interval);
}

void LEAdvertisement1::SetMaxInterval(uint32_t interval) {
    _properties["MaxInterval"] = SimpleDBus::Holder::create_uint32(interval);
}

void LEAdvertisement1::SetTxPower(int16_t power) {
    _properties["TxPower"] = SimpleDBus::Holder::create_int16(power);
}
