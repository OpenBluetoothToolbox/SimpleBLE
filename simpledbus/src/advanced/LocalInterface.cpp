#include <simpledbus/advanced/LocalInterface.h>
#include <simpledbus/base/Exceptions.h>

using namespace SimpleDBus;

LocalInterface::LocalInterface(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path,
                     const std::string& interface_name)
    : InterfaceBase(conn, bus_name, path, interface_name) {}

// ----- LIFE CYCLE -----

// ----- METHODS -----

Message LocalInterface::create_method_call(const std::string& method_name) {
    return Message::create_method_call(_bus_name, _path, _interface_name, method_name);
}

// ----- PROPERTIES -----

Holder LocalInterface::property_get_all() {
    _property_update_mutex.lock();
    Holder result = Holder::create_dict();
    for (const auto& [name, value] : _properties) {
        result.dict_append(Holder::Type::STRING, name, value);
    }
    _property_update_mutex.unlock();
    return result;
}

Holder LocalInterface::property_get(const std::string& property_name) {
    _property_update_mutex.lock();
    auto property = _properties.find(property_name);
    if (property == _properties.end()) {
        throw Exception::PropertyNotFoundException(_path, _interface_name, property_name);
    }
    _property_update_mutex.unlock();
    return property->second;
}

void LocalInterface::property_set(const std::string& property_name, const Holder& value) {
    _property_update_mutex.lock();
    _properties[property_name] = value;

    // TODO: Send property change signal

    _property_update_mutex.unlock();
}

// ----- SIGNALS -----


// ----- MESSAGES -----

void LocalInterface::message_handle(Message& msg) {}
