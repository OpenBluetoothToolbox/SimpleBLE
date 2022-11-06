#include "simplebluez/interfaces/LEAdvertisingManager1.h"

#include <fmt/core.h>

using namespace SimpleBluez;

LEAdvertisingManager1::LEAdvertisingManager1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path)
    : SimpleDBus::Interface(conn, "org.bluez", path, "org.bluez.LEAdvertisingManager1") {

        fmt::print("LEAdvertisingManager1::LEAdvertisingManager1()\n");
    }

void LEAdvertisingManager1::RegisterAdvertisement(std::string advertisement_path) {
    SimpleDBus::Holder properties = SimpleDBus::Holder::create_dict();

    // NOTE: The current documentation doesn't specify any options. Using a placeholder for now.

    auto msg = create_method_call("RegisterAdvertisement");
    msg.append_argument(SimpleDBus::Holder::create_object_path(advertisement_path), "o");
    msg.append_argument(properties, "a{sv}");
    _conn->send(msg);
}

void LEAdvertisingManager1::UnregisterAdvertisement(std::string advertisement_path) {
    auto msg = create_method_call("UnregisterAdvertisement");
    msg.append_argument(SimpleDBus::Holder::create_object_path(advertisement_path), "o");
    _conn->send_with_reply_and_block(msg);
}

uint8_t LEAdvertisingManager1::ActiveInstances(bool refresh) {
    if (refresh) {
        property_refresh("ActiveInstances");
    }

    std::scoped_lock lock(_property_update_mutex);
    return _properties["ActiveInstances"].get_byte();
}

uint8_t LEAdvertisingManager1::SupportedInstances(bool refresh) {
    if (refresh) {
        property_refresh("SupportedInstances");
    }

    std::scoped_lock lock(_property_update_mutex);
    return _properties["SupportedInstances"].get_byte();
}

std::vector<std::string> LEAdvertisingManager1::SupportedIncludes(bool refresh) {
    if (refresh) {
        property_refresh("SupportedIncludes");
    }

    std::scoped_lock lock(_property_update_mutex);
    std::vector<std::string> result;
    for (auto& item : _properties["SupportedIncludes"].get_array()) {
        result.push_back(item.get_string());
    }

    return result;
}


void LEAdvertisingManager1::property_changed(std::string option_name) {}
