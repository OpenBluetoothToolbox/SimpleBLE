#include "simpledbus/advanced/ProxyBase.h"

#include <simpledbus/base/Exceptions.h>
#include <simpledbus/base/Path.h>
#include <algorithm>

using namespace SimpleDBus;

ProxyBase::ProxyBase(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path)
    : _conn(conn), _bus_name(bus_name), _path(path), _valid(true) {}

ProxyBase::~ProxyBase() {
}

std::shared_ptr<InterfaceBase> ProxyBase::interfaces_create(const std::string& name) {
    return std::make_unique<InterfaceBase>(_conn, shared_from_this(), name);
}

std::shared_ptr<ProxyBase> ProxyBase::path_create(const std::string& path) {
    return std::make_shared<ProxyBase>(_conn, _bus_name, path);
}

bool ProxyBase::valid() const { return _valid; }

std::string ProxyBase::path() const { return _path; }

const std::map<std::string, std::shared_ptr<ProxyBase>>& ProxyBase::children() { return _children; }

const std::map<std::string, std::shared_ptr<Interface>>& ProxyBase::interfaces() { return _interfaces; }

// ----- INTERFACE HANDLING -----

bool ProxyBase::interface_exists(const std::string& name) {
    std::scoped_lock lock(_interface_access_mutex);
    return _interfaces.find(name) != _interfaces.end();
}

std::shared_ptr<Interface> ProxyBase::interface_get(const std::string& name) {
    std::scoped_lock lock(_interface_access_mutex);
    if (!interface_exists(name)) {
        throw Exception::InterfaceNotFoundException(_path, name);
    }
    return _interfaces[name];
}

void ProxyBase::interface_insert(const std::string& name) {
    std::scoped_lock lock(_interface_access_mutex);
    _interfaces.insert(std::make_pair(name, interfaces_create(name)));
}

void ProxyBase::interface_remove(const std::string& name) {
    std::scoped_lock lock(_interface_access_mutex);
    _interfaces.erase(name);
}

bool ProxyBase::interfaces_loaded() {
    std::scoped_lock lock(_interface_access_mutex);
    return _interfaces.size() > 0;
}

// ----- CHILD HANDLING -----

bool ProxyBase::path_exists(const std::string& path) {
    std::scoped_lock lock(_child_access_mutex);
    return _children.find(path) != _children.end();
}

std::shared_ptr<ProxyBase> ProxyBase::path_get(const std::string& path) {
    std::scoped_lock lock(_child_access_mutex);
    if (!path_exists(path)) {
        throw Exception::PathNotFoundException(_path, path);
    }
    return _children[path];
}

bool ProxyBase::path_prune() {
    // As children will be extensively accessed, we need to lock the child access mutex.
    std::scoped_lock lock(_child_access_mutex);

    // For each child proxy, check if it can be pruned.
    std::vector<std::string> to_remove;
    for (auto& [child_path, child] : _children) {
        if (child->path_prune() && _children.at(child_path).use_count() == 1) {
            to_remove.push_back(child_path);
        }
    }
    for (auto& child_path : to_remove) {
        _children.erase(child_path);
    }

    // For self to be pruned, the following conditions must be met:
    // 1. The proxy has no children
    // 2. The proxy has no interfaces or all interfaces are disabled.
    if (_children.empty() && !interfaces_loaded()) {
        return true;
    }

    return false;
}

void ProxyBase::path_append_child(const std::string& path, std::shared_ptr<ProxyBase> child) {
    // If the provided path is not a child of the current path, return silently.
    if (!Path::is_child(_path, path)) {
        // TODO: Should an exception be thrown here?
        return;
    }

    // As children will be extensively accessed, we need to lock the child access mutex.
    std::scoped_lock lock(_child_access_mutex);
    _children.emplace(std::make_pair(path, child));
}

// ----- MESSAGE HANDLING -----

void ProxyBase::message_handle(Message& msg) {}

void ProxyBase::message_forward(Message& msg) {
    // If the message is for the current proxy, then forward it to the message handler.
    if (msg.get_path() == _path) {
        message_handle(msg);
        return;
    }

    // If the message is for a child proxy or a descendant, forward it to that child proxy.
    for (auto& [child_path, child] : _children) {
        if (child_path == msg.get_path()) {
            child->message_forward(msg);

            if (msg.get_type() == Message::Type::SIGNAL) {
                on_child_signal_received(child_path);
            }

            return;
        } else if (Path::is_descendant(child_path, msg.get_path())) {
            child->message_forward(msg);
            return;
        }
    }
}

