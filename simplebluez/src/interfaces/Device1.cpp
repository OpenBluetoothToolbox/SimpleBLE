#include "simplebluez/interfaces/Device1.h"

using namespace SimpleBluez;

Device1::Device1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path)
    : SimpleDBus::Interface(conn, "org.bluez", path, "org.bluez.Device1") {}

Device1::~Device1() {
    OnDisconnected.unload();
    OnServicesResolved.unload();
}

void Device1::Connect() {
    auto msg = create_method_call("Connect");
    _conn->send_with_reply_and_block(msg);
}

void Device1::Disconnect() {
    auto msg = create_method_call("Disconnect");
    _conn->send_with_reply_and_block(msg);
}

void Device1::Pair() {
    auto msg = create_method_call("Pair");
    _conn->send_with_reply_and_block(msg);
}

void Device1::CancelPairing() {
    auto msg = create_method_call("CancelPairing");
    _conn->send_with_reply_and_block(msg);
}

uint16_t Device1::Appearance() {
    std::scoped_lock lock(_property_update_mutex);
    return _properties["Appearance"].get_uint16();
}

std::string Device1::Address() {
    std::scoped_lock lock(_property_update_mutex);
    return _properties["Address"].get_string();
}

std::string Device1::AddressType() {
    std::scoped_lock lock(_property_update_mutex);
    return _properties["AddressType"].get_string();
}

std::string Device1::Alias() {
    std::scoped_lock lock(_property_update_mutex);
    return _properties["Alias"].get_string();
}

std::string Device1::Name() {
    std::scoped_lock lock(_property_update_mutex);
    return _properties["Name"].get_string();
}

bool Device1::Paired(bool refresh) {
    if (refresh) {
        property_refresh("Paired");
    }

    std::scoped_lock lock(_property_update_mutex);
    return _properties["Paired"].get_boolean();
}

bool Device1::Connected(bool refresh) {
    if (refresh) {
        property_refresh("Connected");
    }

    std::scoped_lock lock(_property_update_mutex);
    return _properties["Connected"].get_boolean();
}

bool Device1::ServicesResolved(bool refresh) {
    if (refresh) {
        property_refresh("ServicesResolved");
    }

    std::scoped_lock lock(_property_update_mutex);
    return _properties["ServicesResolved"].get_boolean();
}

void Device1::property_changed(std::string option_name) {
    if (option_name == "Connected") {
        if (!Connected(false)) {
            OnDisconnected();
        }
    } else if (option_name == "ServicesResolved") {
        if (ServicesResolved(false)) {
            OnServicesResolved();
        }
    } else if (option_name == "ManufacturerData") {
        ManufacturerData.update_cached_property();
    } else if (option_name == "ServiceData") {
        ServiceData.update_cached_property();
    } else if (option_name == "TxPower") {
        TxPower.update_cached_property();
    }
}
