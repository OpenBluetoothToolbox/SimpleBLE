#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleDBus;

ObjectManager::ObjectManager(std::shared_ptr<Connection> conn, Proxy* proxy)
    : Interface(conn, proxy, "org.freedesktop.DBus.ObjectManager") {}

Holder ObjectManager::GetManagedObjects(bool use_callbacks) {
    Message query_msg = Message::create_method_call(_proxy->bus_name(), _proxy->path(), _interface_name, "GetManagedObjects");
    Message reply_msg = _conn->send_with_reply_and_block(query_msg);
    Holder managed_objects = reply_msg.extract();
    // TODO: Remove immediate callback support.
    if (use_callbacks) {
        auto managed_object = managed_objects.get_dict_object_path();
        for (auto& [path, options] : managed_object) {
            if (InterfacesAdded) {
                InterfacesAdded(path, options);
            }
        }
    }
    return managed_objects;
}

void ObjectManager::message_handle(Message& msg) {
    if (msg.is_signal(_interface_name, "InterfacesAdded")) {
        std::string path = msg.extract().get_string();
        msg.extract_next();
        Holder options = msg.extract();
        if (InterfacesAdded) {
            InterfacesAdded(path, options);
        }
        // TODO: Make a call directly to the proxy to do this?

    } else if (msg.is_signal(_interface_name, "InterfacesRemoved")) {
        std::string path = msg.extract().get_string();
        msg.extract_next();
        Holder options = msg.extract();
        if (InterfacesRemoved) {
            InterfacesRemoved(path, options);
        }
        // TODO: Make a call directly to the proxy to do this?

    } else if (msg.is_method_call(_interface_name, "GetManagedObjects")) {
        SimpleDBus::Holder result = _proxy->path_collect();

        SimpleDBus::Message reply = SimpleDBus::Message::create_method_return(msg);
        reply.append_argument(result, "a{oa{sa{sv}}}");
        _conn->send(reply);
    }
}
