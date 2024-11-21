#include "simplebluez/interfaces/GattService1.h"

using namespace SimpleBluez;

GattService1::GattService1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy)
    : SimpleDBus::Interface(conn, proxy, "org.bluez.GattService1") {}

std::string GattService1::UUID() {
    // As the UUID property doesn't change, we can cache it
    std::scoped_lock lock(_property_update_mutex);
    return _uuid;
}

void GattService1::UUID(const std::string& uuid) {
    std::scoped_lock lock(_property_update_mutex);
    _properties["UUID"] = SimpleDBus::Holder::create_string(uuid);
    _uuid = uuid;
}

bool GattService1::Primary() {
    std::scoped_lock lock(_property_update_mutex);
    return _properties["Primary"].get_boolean();
}

void GattService1::Primary(bool primary) {
    std::scoped_lock lock(_property_update_mutex);
    _properties["Primary"] = SimpleDBus::Holder::create_boolean(primary);
}

std::vector<std::string> GattService1::Characteristics() {
    std::scoped_lock lock(_property_update_mutex);

    std::vector<std::string> characteristics;
    for (SimpleDBus::Holder& characteristic : _properties["Characteristics"].get_array()) {
        characteristics.push_back(characteristic.get_string());
    }

    return characteristics;
}

void GattService1::Characteristics(const std::vector<std::string>& characteristics) {
    std::scoped_lock lock(_property_update_mutex);
    SimpleDBus::Holder characteristics_array = SimpleDBus::Holder::create_array();
    for (const std::string& characteristic : characteristics) {
        characteristics_array.array_append(SimpleDBus::Holder::create_object_path(characteristic));
    }
    _properties["Characteristics"] = characteristics_array;
}

void GattService1::property_changed(std::string option_name) {
    if (option_name == "UUID") {
        std::scoped_lock lock(_property_update_mutex);
        _uuid = _properties["UUID"].get_string();
    }
}
