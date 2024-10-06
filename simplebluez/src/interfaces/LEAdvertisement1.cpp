#include "simplebluez/interfaces/LEAdvertisement1.h"

using namespace SimpleBluez;

LEAdvertisement1::LEAdvertisement1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy)
    : SimpleDBus::Interface(conn, proxy, "org.bluez.LEAdvertisement1") {

    // Set all fields to null or empty values
    SetType("peripheral"); // This is the only required field, so we set a default
    // SetServiceUUIDs({}); // Empty vector

    std::map<uint16_t, std::vector<uint8_t>> data;
    data[0x1024] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    SetManufacturerData(data); // Empty map
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
