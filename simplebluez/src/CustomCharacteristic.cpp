#include <simplebluez/CustomCharacteristic.h>
#include <simplebluez/CustomDescriptor.h>
#include <simplebluez/Exceptions.h>

using namespace SimpleBluez;

CustomCharacteristic::CustomCharacteristic(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name,
                               const std::string& path)
    : Proxy(conn, bus_name, path) {}

CustomCharacteristic::~CustomCharacteristic() {}

std::shared_ptr<SimpleDBus::Proxy> CustomCharacteristic::path_create(const std::string& path) {
    auto child = std::make_shared<CustomDescriptor>(_conn, _bus_name, path);
    return std::static_pointer_cast<SimpleDBus::Proxy>(child);
}

std::shared_ptr<SimpleDBus::Interface> CustomCharacteristic::interfaces_create(const std::string& interface_name) {
    if (interface_name == "org.bluez.GattCharacteristic1") {
        return std::static_pointer_cast<SimpleDBus::Interface>(std::make_shared<GattCharacteristic1>(_conn, this));
    }

    return std::make_shared<SimpleDBus::Interface>(_conn, this, interface_name);
}

std::vector<std::shared_ptr<CustomDescriptor>> CustomCharacteristic::descriptors() { return children_casted<CustomDescriptor>(); }

std::shared_ptr<GattCharacteristic1> CustomCharacteristic::gattcharacteristic1() {
    return std::dynamic_pointer_cast<GattCharacteristic1>(interface_get("org.bluez.GattCharacteristic1"));
}

bool CustomCharacteristic::notifying() { return gattcharacteristic1()->Notifying(); }

std::string CustomCharacteristic::uuid() { return gattcharacteristic1()->UUID(); }

ByteArray CustomCharacteristic::value() { return gattcharacteristic1()->Value(); }

std::vector<std::string> CustomCharacteristic::flags() { return gattcharacteristic1()->Flags(); }

uint16_t CustomCharacteristic::mtu() { return gattcharacteristic1()->MTU(); }

ByteArray CustomCharacteristic::read() { return gattcharacteristic1()->ReadValue(); }

void CustomCharacteristic::write_request(ByteArray value) {
    gattcharacteristic1()->WriteValue(value, GattCharacteristic1::WriteType::REQUEST);
}

void CustomCharacteristic::write_command(ByteArray value) {
    gattcharacteristic1()->WriteValue(value, GattCharacteristic1::WriteType::COMMAND);
}

void CustomCharacteristic::start_notify() { gattcharacteristic1()->StartNotify(); }

void CustomCharacteristic::stop_notify() { gattcharacteristic1()->StopNotify(); }

std::shared_ptr<CustomDescriptor> CustomCharacteristic::get_descriptor(const std::string& uuid) {
    auto descriptors_all = descriptors();

    for (auto& descriptor : descriptors_all) {
        if (descriptor->uuid() == uuid) {
            return descriptor;
        }
    }

    throw Exception::DescriptorNotFoundException(uuid);
}

void CustomCharacteristic::set_on_value_changed(std::function<void(ByteArray new_value)> callback) {
    gattcharacteristic1()->OnValueChanged.load([this, callback]() { callback(gattcharacteristic1()->Value()); });
}

void CustomCharacteristic::clear_on_value_changed() { gattcharacteristic1()->OnValueChanged.unload(); }
