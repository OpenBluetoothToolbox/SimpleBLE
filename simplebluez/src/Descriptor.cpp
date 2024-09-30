#include <simplebluez/Descriptor.h>

using namespace SimpleBluez;

Descriptor::Descriptor(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name,
                       const std::string& path)
    : Proxy(conn, bus_name, path) {}

Descriptor::~Descriptor() {}

std::shared_ptr<SimpleDBus::Interface> Descriptor::interfaces_create(const std::string& interface_name) {
    const std::shared_ptr<SimpleDBus::Proxy> proxy = std::static_pointer_cast<SimpleDBus::Proxy>(shared_from_this());

    if (interface_name == "org.bluez.GattDescriptor1") {
        return std::static_pointer_cast<SimpleDBus::Interface>(std::make_shared<GattDescriptor1>(_conn, proxy));
    }

    return std::make_shared<SimpleDBus::Interface>(_conn, proxy, interface_name);
}

std::shared_ptr<GattDescriptor1> Descriptor::gattdescriptor1() {
    return std::dynamic_pointer_cast<GattDescriptor1>(interface_get("org.bluez.GattDescriptor1"));
}

std::string Descriptor::uuid() { return gattdescriptor1()->UUID(); }

ByteArray Descriptor::value() { return gattdescriptor1()->Value(); }

ByteArray Descriptor::read() { return gattdescriptor1()->ReadValue(); }

void Descriptor::write(ByteArray value) { gattdescriptor1()->WriteValue(value); }

void Descriptor::set_on_value_changed(std::function<void(ByteArray new_value)> callback) {
    gattdescriptor1()->OnValueChanged.load([this, callback]() { callback(gattdescriptor1()->Value()); });
}

void Descriptor::clear_on_value_changed() { gattdescriptor1()->OnValueChanged.unload(); }
