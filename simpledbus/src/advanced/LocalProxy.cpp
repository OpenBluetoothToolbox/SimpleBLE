#include "simpledbus/advanced/LocalProxy.h"

#include <simpledbus/base/Exceptions.h>
#include <simpledbus/base/Path.h>
#include <algorithm>
#include <iostream> // TODO Remove once I'm done.

using namespace SimpleDBus;

LocalProxy::LocalProxy(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path)
    : ProxyBase(conn, bus_name, path) {}

LocalProxy::~LocalProxy() {
}

std::shared_ptr<LocalInterface> LocalProxy::interfaces_create(const std::string& name) {
    return std::make_unique<LocalInterface>(_conn, _bus_name, _path, name);
}

std::shared_ptr<LocalProxy> LocalProxy::path_create(const std::string& path) {
    return std::make_shared<LocalProxy>(_conn, _bus_name, path);
}

const std::map<std::string, std::shared_ptr<LocalProxy>>& LocalProxy::children() { return _children; }

const std::map<std::string, std::shared_ptr<LocalInterface>>& LocalProxy::interfaces() { return _interfaces; }

// ----- INTROSPECTION -----
std::string LocalProxy::introspect() {
    auto query_msg = Message::create_method_call(_bus_name, _path, "org.freedesktop.DBus.Introspectable", "Introspect");
    auto reply_msg = _conn->send_with_reply_and_block(query_msg);
    return reply_msg.extract().get_string();
}

// ----- INTERFACE HANDLING -----

bool LocalProxy::interface_exists(const std::string& name) {
    std::scoped_lock lock(_interface_access_mutex);
    return _interfaces.find(name) != _interfaces.end();
}

std::shared_ptr<LocalInterface> LocalProxy::interface_get(const std::string& name) {
    std::scoped_lock lock(_interface_access_mutex);
    if (!interface_exists(name)) {
        throw Exception::InterfaceNotFoundException(_path, name);
    }
    return _interfaces[name];
}

void LocalProxy::interfaces_load(const std::string& interface_name, std::shared_ptr<LocalInterface> interface) {
    std::scoped_lock lock(_interface_access_mutex);
    _interfaces.emplace(std::make_pair(interface_name, interface));
}

void LocalProxy::interfaces_unload(const std::string& interface_name) {
    std::scoped_lock lock(_interface_access_mutex);
    _interfaces.erase(interface_name);
    // TODO: Do we need the "loaded" mechanism to keep the object alive in case some other object still holds a reference to it?
}

// ----- CHILD HANDLING -----

bool LocalProxy::path_exists(const std::string& path) {
    std::scoped_lock lock(_child_access_mutex);
    return _children.find(path) != _children.end();
}

std::shared_ptr<LocalProxy> LocalProxy::path_get(const std::string& path) {
    std::scoped_lock lock(_child_access_mutex);
    if (!path_exists(path)) {
        throw Exception::PathNotFoundException(_path, path);
    }
    return _children[path];
}

void LocalProxy::path_add(const std::string& path, std::shared_ptr<LocalProxy> child) {
    // If the path is not a child of the current path, then we can't add it.
    if (!Path::is_descendant(_path, path)) {
        // TODO: Throw exception
        return;
    }

    // As children will be extensively accessed, we need to lock the child access mutex.
    std::scoped_lock lock(_child_access_mutex);

    if (Path::is_child(_path, path)) {
        // If the path is a direct child of the proxy path, add it to the children map.
        _children.emplace(std::make_pair(path, child));
    } else {
        // If the new path is for a descendant of the current proxy, check if there is a child proxy for it.
        auto child_result = std::find_if(
            _children.begin(), _children.end(),
            [path](const std::pair<std::string, std::shared_ptr<LocalProxy>>& child_data) -> bool {
                return Path::is_descendant(child_data.first, path);
            });

        if (child_result != _children.end()) {
            // If there is a child proxy for the new path, forward it to that child proxy.
            child_result->second->path_add(path, child);
        } else {
            // If there is no child proxy for the new path, create the child and forward the path to it.
            // This path will be taken if an empty proxy object needs to be created for an intermediate path.
            std::string next_child_path = Path::next_child(_path, path);
            std::shared_ptr<LocalProxy> next_child = path_create(next_child_path);
            _children.emplace(std::make_pair(next_child_path, next_child));
            next_child->path_add(path, child);
        }
    }
}

bool LocalProxy::path_remove(const std::string& path) {
    // `options` contains an array of strings of the interfaces that need to be removed.

    if (path == _path) {
        _valid = false;
        return path_prune();
    }

    // If the path is not the current path nor a descendant, then there's nothing to do
    if (!Path::is_descendant(_path, path)) {
        return false;
    }

    // As children will be extensively accessed, we need to lock the child access mutex.
    std::scoped_lock lock(_child_access_mutex);

    // If the path is a direct child of the proxy path, forward the request to the child proxy.
    std::string child_path = Path::next_child(_path, path);
    if (path_exists(child_path)) {
        bool must_erase = _children.at(child_path)->path_remove(path);

        // if the child proxy is no longer needed and there is only one active instance of the child proxy,
        // then remove it.
        if (must_erase && _children.at(child_path).use_count() == 1) {
            _children.erase(child_path);
        }
    }

    return false;
}

bool LocalProxy::path_prune() {
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
    if (_children.empty()) {
        return true;
    }

    return false;
}

// ----- MESSAGE HANDLING -----

void LocalProxy::message_handle(Message& msg) {
    // If the message is involves a property change, forward it to the correct interface.
    if (interface_exists(msg.get_interface())) {
        interface_get(msg.get_interface())->message_handle(msg);
        return;
    }

    std::cout << "LocalProxy::Unhandled message: " << msg.get_path() << std::endl;
}

void LocalProxy::message_forward(Message& msg) {
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
