#include "simpledbus/advanced/ProxyBase.h"

using namespace SimpleDBus;

ProxyBase::ProxyBase(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path)
    : _conn(conn), _bus_name(bus_name), _path(path), _registered(false) {
    register_object_path();
}

ProxyBase::~ProxyBase() { unregister_object_path(); }

bool ProxyBase::valid() const { return _valid; }

void ProxyBase::invalidate() {
    _valid = false;
    unregister_object_path();
}

std::string ProxyBase::path() const { return _path; }

void ProxyBase::register_object_path() {
    if (!_registered) {
        _conn->register_object_path(_path, [this](Message& msg) { this->message_handle(msg); });
        _registered = true;
    }
}

void ProxyBase::unregister_object_path() {
    if (_registered) {
        _conn->unregister_object_path(_path);
        _registered = false;
    }
}

void ProxyBase::message_handle(Message& msg) {}
