#include <simplebluez/CustomCharacteristic.h>
#include <simplebluez/CustomDescriptor.h>
#include <simplebluez/Exceptions.h>

#include <simpledbus/interfaces/Properties.h>

using namespace SimpleBluez;

CustomCharacteristic::CustomCharacteristic(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name,
                               const std::string& path)
    : Proxy(conn, bus_name, path) {

    _interfaces.emplace(std::make_pair("org.bluez.GattCharacteristic1", std::make_shared<GattCharacteristic1>(conn, this)));
    }

CustomCharacteristic::~CustomCharacteristic() {}

std::shared_ptr<CustomDescriptor> CustomCharacteristic::create_descriptor() {
    const std::string descriptor_path = _path + "/descriptor" + std::to_string(_children.size());
    auto descriptor = std::make_shared<CustomDescriptor>(_conn, _bus_name, descriptor_path);
    path_append_child(descriptor_path, std::static_pointer_cast<SimpleDBus::Proxy>(descriptor));
    return descriptor;
}

void CustomCharacteristic::remove_descriptor(const std::string& path) {
    const std::string descriptor_path = _path + "/" + path;
    path_remove_child(descriptor_path);
}

std::shared_ptr<GattCharacteristic1> CustomCharacteristic::gattcharacteristic1() {
    return std::dynamic_pointer_cast<GattCharacteristic1>(interface_get("org.bluez.GattCharacteristic1"));
}

bool CustomCharacteristic::notifying() { return gattcharacteristic1()->Notifying(); }

std::string CustomCharacteristic::uuid() { return gattcharacteristic1()->UUID(); }

void CustomCharacteristic::uuid(const std::string& uuid) { gattcharacteristic1()->UUID(uuid); }

std::string CustomCharacteristic::service() { return gattcharacteristic1()->Service(); }

void CustomCharacteristic::service(const std::string& service) { gattcharacteristic1()->Service(service); }

ByteArray CustomCharacteristic::value() { return gattcharacteristic1()->Value(); }

void CustomCharacteristic::value(const ByteArray& value) {
    gattcharacteristic1()->Value(value);
    SimpleDBus::Holder value_h = SimpleDBus::Holder::create_array();
    for (uint8_t byte : value) {
        value_h.array_append(SimpleDBus::Holder::create_byte(byte));
    }

    std::map<std::string, SimpleDBus::Holder> changed_properties;
    changed_properties["Value"] = value_h;

    std::shared_ptr<SimpleDBus::Properties> properties = std::dynamic_pointer_cast<SimpleDBus::Properties>(interface_get("org.freedesktop.DBus.Properties"));
    properties->PropertiesChanged("org.bluez.GattCharacteristic1", changed_properties);
 }

std::vector<std::string> CustomCharacteristic::flags() { return gattcharacteristic1()->Flags(); }

void CustomCharacteristic::flags(const std::vector<std::string>& flags) { gattcharacteristic1()->Flags(flags); }

uint16_t CustomCharacteristic::mtu() { return gattcharacteristic1()->MTU(); }

void CustomCharacteristic::mtu(uint16_t mtu) { gattcharacteristic1()->MTU(mtu); }

ByteArray CustomCharacteristic::read() { return gattcharacteristic1()->ReadValue(); }

void CustomCharacteristic::write_request(ByteArray value) {
    gattcharacteristic1()->WriteValue(value, GattCharacteristic1::WriteType::REQUEST);
}

void CustomCharacteristic::write_command(ByteArray value) {
    gattcharacteristic1()->WriteValue(value, GattCharacteristic1::WriteType::COMMAND);
}

void CustomCharacteristic::start_notify() { gattcharacteristic1()->StartNotify(); }

void CustomCharacteristic::stop_notify() { gattcharacteristic1()->StopNotify(); }

void CustomCharacteristic::set_on_value_changed(std::function<void(ByteArray new_value)> callback) {
    gattcharacteristic1()->OnValueChanged.load([this, callback]() { callback(gattcharacteristic1()->Value()); });
}

void CustomCharacteristic::clear_on_value_changed() { gattcharacteristic1()->OnValueChanged.unload(); }
