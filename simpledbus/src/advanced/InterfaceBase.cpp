#include <simpledbus/advanced/InterfaceBase.h>

using namespace SimpleDBus;

InterfaceBase::InterfaceBase(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path,
                     const std::string& interface_name)
    : _conn(conn), _bus_name(bus_name), _path(path), _interface_name(interface_name) {}
