#include "simplebluez/interfaces/Adapter1.h"

using namespace SimpleBluez;

Adapter1::Adapter1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path)
    : SimpleDBus::Interface(conn, "org.bluez", path, "org.bluez.Adapter1") {}

void Adapter1::StartDiscovery() {
    auto msg = create_method_call("StartDiscovery");
    _conn->send_with_reply_and_block(msg);
}

void Adapter1::StopDiscovery() {
    auto msg = create_method_call("StopDiscovery");
    _conn->send_with_reply_and_block(msg);
    // NOTE: It might take a few seconds until the peripheral reports that is has actually stopped discovering.
}

SimpleDBus::Holder Adapter1::GetDiscoveryFilters() {
    auto msg = create_method_call("GetDiscoveryFilters");
    SimpleDBus::Message reply_msg = _conn->send_with_reply_and_block(msg);
    SimpleDBus::Holder discovery_filters = reply_msg.extract();
    return discovery_filters;
}

void Adapter1::SetDiscoveryFilter(DiscoveryFilter filter) {
    SimpleDBus::Holder properties = SimpleDBus::Holder::create_array();

    if (filter.UUIDs.size() > 0) {
        SimpleDBus::Holder uuids = SimpleDBus::Holder::create_array();
        for (size_t i = 0; i < filter.UUIDs.size(); i++) {
            uuids.array_append(SimpleDBus::Holder::create_string(filter.UUIDs.at(i)));
        }
        SimpleDBus::Holder dict = SimpleDBus::Holder::create_dict();
        dict.dict_append(SimpleDBus::Holder::Type::STRING, "UUIDs", uuids);
        properties.array_append(dict);
    }

    if (filter.RSSI.has_value()) {
        SimpleDBus::Holder dict = SimpleDBus::Holder::create_dict();
        dict.dict_append(SimpleDBus::Holder::Type::STRING, "RSSI",
                         SimpleDBus::Holder::create_int16(filter.RSSI.value()));
        properties.array_append(dict);
    }

    if (filter.Pathloss.has_value()) {
        SimpleDBus::Holder dict = SimpleDBus::Holder::create_dict();
        dict.dict_append(SimpleDBus::Holder::Type::STRING, "Pathloss",
                         SimpleDBus::Holder::create_uint16(filter.Pathloss.value()));
        properties.array_append(dict);
    }

    {
        SimpleDBus::Holder dict = SimpleDBus::Holder::create_dict();
        switch (filter.Transport) {
            case DiscoveryFilter::TransportType::AUTO: {
                dict.dict_append(SimpleDBus::Holder::Type::STRING, "Transport",
                                 SimpleDBus::Holder::create_string("auto"));
                break;
            }
            case DiscoveryFilter::TransportType::BREDR: {
                dict.dict_append(SimpleDBus::Holder::Type::STRING, "Transport",
                                 SimpleDBus::Holder::create_string("bredr"));
                break;
            }
            case DiscoveryFilter::TransportType::LE: {
                dict.dict_append(SimpleDBus::Holder::Type::STRING, "Transport",
                                 SimpleDBus::Holder::create_string("le"));
                break;
            }
        }
        properties.array_append(dict);
    }

    if (!filter.DuplicateData) {
        SimpleDBus::Holder dict = SimpleDBus::Holder::create_dict();
        dict.dict_append(SimpleDBus::Holder::Type::STRING, "DuplicateData",
                               SimpleDBus::Holder::create_boolean(false));
        properties.array_append(dict);
    }

    if (filter.Discoverable) {
        SimpleDBus::Holder dict = SimpleDBus::Holder::create_dict();
        dict.dict_append(SimpleDBus::Holder::Type::STRING, "Discoverable",
                               SimpleDBus::Holder::create_boolean(false));
        properties.array_append(dict);
    }

    if (filter.Pattern.size() > 0) {
        SimpleDBus::Holder dict = SimpleDBus::Holder::create_dict();
        dict.dict_append(SimpleDBus::Holder::Type::STRING, "Pattern",
                               SimpleDBus::Holder::create_string(filter.Pattern));
        properties.array_append(dict);
    }

    auto msg = create_method_call("SetDiscoveryFilter");
    msg.append_argument(properties, "a{sv}");
    _conn->send_with_reply_and_block(msg);
}

void Adapter1::RemoveDevice(std::string device_path) {
    auto msg = create_method_call("RemoveDevice");
    msg.append_argument(SimpleDBus::Holder::create_object_path(device_path), "o");
    _conn->send_with_reply_and_block(msg);
}

bool Adapter1::Discovering(bool refresh) {
    if (refresh) {
        property_refresh("Discovering");
    }

    std::scoped_lock lock(_property_update_mutex);
    return _properties["Discovering"].get_boolean();
}

bool Adapter1::Powered(bool refresh) {
    if (refresh) {
        property_refresh("Powered");
    }

    std::scoped_lock lock(_property_update_mutex);
    return _properties["Powered"].get_boolean();
}

std::string Adapter1::Address() {
    std::scoped_lock lock(_property_update_mutex);
    return _properties["Address"].get_string();
}

void Adapter1::property_changed(std::string option_name) {}
