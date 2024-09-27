#include "simplebluez/interfaces/LEAdvertisement1.h"

using namespace SimpleBluez;

LEAdvertisement1::LEAdvertisement1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path)
    : SimpleDBus::Interface(conn, "org.bluez", path, "org.bluez.LEAdvertisement1") {}

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

void LEAdvertisement1::property_changed(std::string option_name) {}

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