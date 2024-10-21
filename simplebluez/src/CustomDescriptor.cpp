#include <simplebluez/CustomDescriptor.h>

using namespace SimpleBluez;

CustomDescriptor::CustomDescriptor(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name,
                                   const std::string& path)
    : Proxy(conn, bus_name, path) {
    _interfaces.emplace(std::make_pair("org.bluez.GattDescriptor1", std::make_shared<GattDescriptor1>(conn, this)));
}

CustomDescriptor::~CustomDescriptor() {}

std::shared_ptr<GattDescriptor1> CustomDescriptor::gattdescriptor1() {
    return std::dynamic_pointer_cast<GattDescriptor1>(interface_get("org.bluez.GattDescriptor1"));
}

std::string CustomDescriptor::uuid() { return gattdescriptor1()->UUID(); }

ByteArray CustomDescriptor::value() { return gattdescriptor1()->Value(); }

ByteArray CustomDescriptor::read() { return gattdescriptor1()->ReadValue(); }

void CustomDescriptor::write(ByteArray value) { gattdescriptor1()->WriteValue(value); }

void CustomDescriptor::set_on_value_changed(std::function<void(ByteArray new_value)> callback) {
    gattdescriptor1()->OnValueChanged.load([this, callback]() { callback(gattdescriptor1()->Value()); });
}

void CustomDescriptor::clear_on_value_changed() { gattdescriptor1()->OnValueChanged.unload(); }
