#include "simplebluez/interfaces/LEAdvertisement1.h"

using namespace SimpleBluez;

LEAdvertisement1::LEAdvertisement1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy)
    : SimpleDBus::Interface(conn, proxy, "org.bluez.LEAdvertisement1") {

    Type("peripheral"); // This is the only required field, so we set a default
}

void LEAdvertisement1::Type(const std::string& type) {
    _properties["Type"] = SimpleDBus::Holder::create_string(type);
}

std::string LEAdvertisement1::Type() {
    return _properties["Type"].get_string();
}

void LEAdvertisement1::ServiceUUIDs(const std::vector<std::string>& uuids) {
    SimpleDBus::Holder uuids_holder = SimpleDBus::Holder::create_array();
    for (const auto& uuid : uuids) {
        uuids_holder.array_append(SimpleDBus::Holder::create_string(uuid));
    }
    _properties["ServiceUUIDs"] = uuids_holder;
}

std::vector<std::string> LEAdvertisement1::ServiceUUIDs() {
    std::vector<std::string> uuids;
    auto uuids_holder = _properties["ServiceUUIDs"];
    for (const auto& uuid : uuids_holder.get_array()) {
        uuids.push_back(uuid.get_string());
    }
    return uuids;
}

void LEAdvertisement1::ManufacturerData(const std::map<uint16_t, std::vector<uint8_t>>& data) {
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

std::map<uint16_t, std::vector<uint8_t>> LEAdvertisement1::ManufacturerData() {
    std::map<uint16_t, std::vector<uint8_t>> data;
    auto data_holder = _properties["ManufacturerData"];
    for (const auto& [key, value] : data_holder.get_dict_uint16()) {
        std::vector<uint8_t> bytes;
        for (const auto& byte : value.get_array()) {
            bytes.push_back(byte.get_byte());
        }
        data[key] = bytes;
    }
    return data;
}

void LEAdvertisement1::ServiceData(const std::map<std::string, std::vector<uint8_t>>& data) {
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

std::map<std::string, std::vector<uint8_t>> LEAdvertisement1::ServiceData() {
    std::map<std::string, std::vector<uint8_t>> data;
    auto data_holder = _properties["ServiceData"];
    for (const auto& [key, value] : data_holder.get_dict_string()) {
        std::vector<uint8_t> bytes;
        for (const auto& byte : value.get_array()) {
            bytes.push_back(byte.get_byte());
        }
        data[key] = bytes;
    }
    return data;
}

void LEAdvertisement1::SolicitUUIDs(const std::vector<std::string>& uuids) {
    SimpleDBus::Holder uuids_holder = SimpleDBus::Holder::create_array();
    for (const auto& uuid : uuids) {
        uuids_holder.array_append(SimpleDBus::Holder::create_string(uuid));
    }
    _properties["SolicitUUIDs"] = uuids_holder;
}

std::vector<std::string> LEAdvertisement1::SolicitUUIDs() {
    std::vector<std::string> uuids;
    auto uuids_holder = _properties["SolicitUUIDs"];
    for (const auto& uuid : uuids_holder.get_array()) {
        uuids.push_back(uuid.get_string());
    }
    return uuids;
}

void LEAdvertisement1::Data(const std::map<uint8_t, std::vector<uint8_t>>& data) {
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

std::map<uint8_t, std::vector<uint8_t>> LEAdvertisement1::Data() {
    std::map<uint8_t, std::vector<uint8_t>> data;
    auto data_holder = _properties["Data"];
    for (const auto& [key, value] : data_holder.get_dict_uint8()) {
        std::vector<uint8_t> bytes;
        for (const auto& byte : value.get_array()) {
            bytes.push_back(byte.get_byte());
        }
        data[key] = bytes;
    }
    return data;
}

void LEAdvertisement1::Discoverable(bool discoverable) {
    _properties["Discoverable"] = SimpleDBus::Holder::create_boolean(discoverable);
}

bool LEAdvertisement1::Discoverable() {
    return _properties["Discoverable"].get_boolean();
}

void LEAdvertisement1::DiscoverableTimeout(uint16_t timeout) {
    _properties["DiscoverableTimeout"] = SimpleDBus::Holder::create_uint16(timeout);
}

uint16_t LEAdvertisement1::DiscoverableTimeout() {
    return _properties["DiscoverableTimeout"].get_uint16();
}

void LEAdvertisement1::Includes(const std::vector<std::string>& includes) {
    SimpleDBus::Holder includes_holder = SimpleDBus::Holder::create_array();
    for (const auto& include : includes) {
        includes_holder.array_append(SimpleDBus::Holder::create_string(include));
    }
    _properties["Includes"] = includes_holder;
}

std::vector<std::string> LEAdvertisement1::Includes() {
    std::vector<std::string> includes;
    auto includes_holder = _properties["Includes"];
    for (const auto& include : includes_holder.get_array()) {
        includes.push_back(include.get_string());
    }
    return includes;
}

void LEAdvertisement1::LocalName(const std::string& name) {
    _properties["LocalName"] = SimpleDBus::Holder::create_string(name);
}

std::string LEAdvertisement1::LocalName() {
    return _properties["LocalName"].get_string();
}

void LEAdvertisement1::Appearance(uint16_t appearance) {
    _properties["Appearance"] = SimpleDBus::Holder::create_uint16(appearance);
}

uint16_t LEAdvertisement1::Appearance() {
    return _properties["Appearance"].get_uint16();
}

void LEAdvertisement1::Duration(uint16_t duration) {
    _properties["Duration"] = SimpleDBus::Holder::create_uint16(duration);
}

uint16_t LEAdvertisement1::Duration() {
    return _properties["Duration"].get_uint16();
}

void LEAdvertisement1::Timeout(uint16_t timeout) {
    _properties["Timeout"] = SimpleDBus::Holder::create_uint16(timeout);
}

uint16_t LEAdvertisement1::Timeout() {
    return _properties["Timeout"].get_uint16();
}

void LEAdvertisement1::SecondaryChannel(const std::string& channel) {
    _properties["SecondaryChannel"] = SimpleDBus::Holder::create_string(channel);
}

std::string LEAdvertisement1::SecondaryChannel() {
    return _properties["SecondaryChannel"].get_string();
}

void LEAdvertisement1::MinInterval(uint32_t interval) {
    _properties["MinInterval"] = SimpleDBus::Holder::create_uint32(interval);
}

uint32_t LEAdvertisement1::MinInterval() {
    return _properties["MinInterval"].get_uint32();
}

void LEAdvertisement1::MaxInterval(uint32_t interval) {
    _properties["MaxInterval"] = SimpleDBus::Holder::create_uint32(interval);
}

uint32_t LEAdvertisement1::MaxInterval() {
    return _properties["MaxInterval"].get_uint32();
}

void LEAdvertisement1::TxPower(int16_t power) {
    _properties["TxPower"] = SimpleDBus::Holder::create_int16(power);
}

int16_t LEAdvertisement1::TxPower() {
    return _properties["TxPower"].get_int16();
}

void LEAdvertisement1::IncludeTxPower(bool include) {
    _properties["IncludeTxPower"] = SimpleDBus::Holder::create_boolean(include);
}

bool LEAdvertisement1::IncludeTxPower() {
    return _properties["IncludeTxPower"].get_boolean();
}

void LEAdvertisement1::message_handle(SimpleDBus::Message& msg) {
    if (msg.is_method_call(_interface_name, "Release")) {
        OnRelease();

        SimpleDBus::Message reply = SimpleDBus::Message::create_method_return(msg);
        _conn->send(reply);

    }
}