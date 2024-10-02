#include <simpledbus/interfaces/Properties.h>

using namespace SimpleDBus;

Properties::Properties(std::shared_ptr<Connection> conn, std::shared_ptr<Proxy> proxy)
    : Interface(conn, proxy, "org.freedesktop.DBus.Properties") {}

Holder Properties::Get(const std::string& interface_name, const std::string& property_name) {
    auto proxy = _proxy.lock();
    Message query_msg = Message::create_method_call(proxy->bus_name(), proxy->path(), "org.freedesktop.DBus.Properties",
                                                    "Get");

    Holder h_interface = Holder::create_string(interface_name);
    query_msg.append_argument(h_interface, "s");

    Holder h_name = Holder::create_string(property_name);
    query_msg.append_argument(h_name, "s");

    Message reply_msg = _conn->send_with_reply_and_block(query_msg);
    Holder result = reply_msg.extract();
    return result;
}

Holder Properties::GetAll(const std::string& interface_name) {
    auto proxy = _proxy.lock();
    Message query_msg = Message::create_method_call(proxy->bus_name(), proxy->path(), "org.freedesktop.DBus.Properties",
                                                    "GetAll");

    Holder h_interface = Holder::create_string(interface_name);
    query_msg.append_argument(h_interface, "s");

    Message reply_msg = _conn->send_with_reply_and_block(query_msg);
    Holder result = reply_msg.extract();
    return result;
}

void Properties::Set(const std::string& interface_name, const std::string& property_name, const Holder& value) {
    auto proxy = _proxy.lock();
    Message query_msg = Message::create_method_call(proxy->bus_name(), proxy->path(), "org.freedesktop.DBus.Properties",
                                                    "Set");

    Holder h_interface = Holder::create_string(interface_name);
    query_msg.append_argument(h_interface, "s");

    Holder h_name = Holder::create_string(property_name);
    query_msg.append_argument(h_name, "s");

    query_msg.append_argument(value, "v");

    _conn->send_with_reply_and_block(query_msg);
}

void Properties::message_handle(Message& msg) {
    if (msg.is_method_call(_interface_name, "GetAll")) {
        Holder interface_h = msg.extract();
        std::string iface_name = interface_h.get_string();

        auto proxy = _proxy.lock();
        std::shared_ptr<Interface> interface = proxy->interface_get(iface_name);
        Holder result = interface->property_collect();

        SimpleDBus::Message reply = SimpleDBus::Message::create_method_return(msg);
        reply.append_argument(result, "a{sv}");
        _conn->send(reply);
        
    } else if (msg.is_method_call(_interface_name, "Get")) {
        Holder interface_h = msg.extract();
        std::string iface_name = interface_h.get_string();
        msg.extract_next();

        Holder property_h = msg.extract();
        std::string property_name = property_h.get_string();

        auto proxy = _proxy.lock();
        std::shared_ptr<Interface> interface = proxy->interface_get(iface_name);

        Holder result = interface->property_collect_single(property_name);

        SimpleDBus::Message reply = SimpleDBus::Message::create_method_return(msg);
        reply.append_argument(result, "v");
        _conn->send(reply);

    } else if (msg.is_method_call(_interface_name, "Set")) {
        Holder interface_h = msg.extract();
        std::string iface_name = interface_h.get_string();
        msg.extract_next();

        Holder property_h = msg.extract();
        std::string property_name = property_h.get_string();
        msg.extract_next();

        Holder value_h = msg.extract();

        auto proxy = _proxy.lock();
        std::shared_ptr<Interface> interface = proxy->interface_get(iface_name);
        interface->property_modify(property_name, value_h);

    } else if (msg.is_signal(_interface_name, "PropertiesChanged")) {
        Holder interface_h = msg.extract();
        std::string iface_name = interface_h.get_string();
        msg.extract_next();
        Holder changed_properties = msg.extract();
        msg.extract_next();
        Holder invalidated_properties = msg.extract();

        auto proxy = _proxy.lock();
        // If the interface is not loaded, then ignore the message.
        if (!proxy->interface_exists(iface_name)) {
            return;
        }

        proxy->interface_get(iface_name)->signal_property_changed(changed_properties, invalidated_properties);
    }
}
