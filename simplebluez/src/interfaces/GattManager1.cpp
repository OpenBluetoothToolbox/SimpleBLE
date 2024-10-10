#include "simplebluez/interfaces/GattManager1.h"

using namespace SimpleBluez;

GattManager1::GattManager1(std::shared_ptr<SimpleDBus::Connection> conn, SimpleDBus::Proxy* proxy)
    : SimpleDBus::Interface(conn, proxy, "org.bluez.GattManager1") {}

void GattManager1::RegisterApplication(std::string application_path) {
    SimpleDBus::Holder properties = SimpleDBus::Holder::create_dict();

    // NOTE: The current documentation doesn't specify any options. Using a placeholder for now.

    auto msg = create_method_call("RegisterApplication");
    msg.append_argument(SimpleDBus::Holder::create_object_path(application_path), "o");
    msg.append_argument(properties, "a{sv}");

    // TODO: We need a way to get an async reply for this.
    // TODO: Can this work with the new routing system?
    _conn->send(msg);
}

void GattManager1::UnregisterApplication(std::string application_path) {
    auto msg = create_method_call("UnregisterApplication");
    msg.append_argument(SimpleDBus::Holder::create_object_path(application_path), "o");
    _conn->send_with_reply_and_block(msg);
}
