#include <simpledbus/advanced/InterfaceBase.h>
#include <simpledbus/base/Exceptions.h>

using namespace SimpleDBus;

InterfaceBase::InterfaceBase(std::shared_ptr<Connection> conn, std::shared_ptr<ProxyBase> proxy,
                             const std::string& interface_name)
    : _conn(conn), _proxy(proxy), _interface_name(interface_name) {}

// ----- LIFE CYCLE -----

// ----- METHODS -----


// ----- PROPERTIES -----


// ----- SIGNALS -----


// ----- MESSAGES -----

void Interface::message_handle(Message& msg) {}
