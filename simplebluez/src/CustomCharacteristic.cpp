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

std::string CustomCharacteristic::uuid() { return gattcharacteristic1()->UUID(); }

void CustomCharacteristic::uuid(const std::string& uuid) { gattcharacteristic1()->UUID(uuid); }

std::string CustomCharacteristic::service() { return gattcharacteristic1()->Service(); }

void CustomCharacteristic::service(const std::string& service) { gattcharacteristic1()->Service(service); }

ByteArray CustomCharacteristic::value() { return gattcharacteristic1()->Value(); }

void CustomCharacteristic::value(const ByteArray& value) { gattcharacteristic1()->Value(value); }

bool CustomCharacteristic::notifying() { return gattcharacteristic1()->Notifying(false); }

std::vector<std::string> CustomCharacteristic::flags() { return gattcharacteristic1()->Flags(); }

void CustomCharacteristic::flags(const std::vector<std::string>& flags) { gattcharacteristic1()->Flags(flags); }

uint16_t CustomCharacteristic::mtu() { return gattcharacteristic1()->MTU(); }

void CustomCharacteristic::mtu(uint16_t mtu) { gattcharacteristic1()->MTU(mtu); }

void CustomCharacteristic::set_on_read_value(std::function<void()> callback) {
    gattcharacteristic1()->OnReadValue.load([this, callback]() { callback(); });
}

void CustomCharacteristic::clear_on_read_value() { gattcharacteristic1()->OnReadValue.unload(); }

void CustomCharacteristic::set_on_write_value(std::function<void(ByteArray value)> callback) {
    gattcharacteristic1()->OnWriteValue.load([this, callback](const ByteArray& value) { callback(value); });
}

void CustomCharacteristic::clear_on_write_value() { gattcharacteristic1()->OnWriteValue.unload(); }

void CustomCharacteristic::set_on_notify(std::function<void(bool)> callback) {
    gattcharacteristic1()->OnStartNotify.load([this, callback]() { callback(true); });
    gattcharacteristic1()->OnStopNotify.load([this, callback]() { callback(false); });
}

void CustomCharacteristic::clear_on_notify() {
    gattcharacteristic1()->OnStartNotify.unload();
    gattcharacteristic1()->OnStopNotify.unload();
}
