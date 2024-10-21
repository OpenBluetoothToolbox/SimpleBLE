#include "simpledbus/advanced/Proxy.h"

#include <simpledbus/base/Exceptions.h>
#include <simpledbus/base/Path.h>
#include <algorithm>

#include <simpledbus/interfaces/Properties.h>

using namespace SimpleDBus;

Proxy::Proxy(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path)
    : _conn(conn), _bus_name(bus_name), _path(path), _valid(true), _registered(false) {
    register_object_path();

    _interfaces.emplace(std::make_pair("org.freedesktop.DBus.Properties", std::make_shared<Properties>(conn, this)));
}

Proxy::~Proxy() {
    unregister_object_path();
    on_child_created.unload();
    on_child_signal_received.unload();
}

std::shared_ptr<Interface> Proxy::interfaces_create(const std::string& name) {
    return std::make_shared<Interface>(_conn, this, name);
}

std::shared_ptr<Proxy> Proxy::path_create(const std::string& path) {
    return std::make_shared<Proxy>(_conn, _bus_name, path);
}

bool Proxy::valid() const { return _valid; }

void Proxy::invalidate() {
    _valid = false;
    unregister_object_path();
}

std::string Proxy::path() const { return _path; }

std::string Proxy::bus_name() const { return _bus_name; }

const std::map<std::string, std::shared_ptr<Proxy>>& Proxy::children() { return _children; }

const std::map<std::string, std::shared_ptr<Interface>>& Proxy::interfaces() { return _interfaces; }

// ----- PATH HANDLING -----

void Proxy::register_object_path() {
    if (!_registered && _conn->register_object_path(_path, [this](Message& msg) { this->message_handle(msg); })) {
        _registered = true;
    }
}

void Proxy::unregister_object_path() {
    if (_registered && _conn->unregister_object_path(_path)) {
        _registered = false;
    }
}


// ----- INTROSPECTION -----

std::string Proxy::introspect() {
    auto query_msg = Message::create_method_call(_bus_name, _path, "org.freedesktop.DBus.Introspectable", "Introspect");
    auto reply_msg = _conn->send_with_reply_and_block(query_msg);
    return reply_msg.extract().get_string();
}

// ----- INTERFACE HANDLING -----

bool Proxy::interface_exists(const std::string& name) {
    std::scoped_lock lock(_interface_access_mutex);
    return _interfaces.find(name) != _interfaces.end();
}

std::shared_ptr<Interface> Proxy::interface_get(const std::string& name) {
    std::scoped_lock lock(_interface_access_mutex);
    if (!interface_exists(name)) {
        throw Exception::InterfaceNotFoundException(_path, name);
    }
    return _interfaces[name];
}

size_t Proxy::interfaces_count() {
    size_t count = 0;
    std::scoped_lock lock(_interface_access_mutex);
    for (auto& [iface_name, interface] : _interfaces) {
        if (interface->is_loaded()) {
            count++;
        }
    }
    return count;
}

void Proxy::interfaces_load(Holder managed_interfaces) {
    auto managed_interface = managed_interfaces.get_dict_string();

    std::scoped_lock lock(_interface_access_mutex);
    for (auto& [iface_name, options] : managed_interface) {
        // If the interface has not been loaded, load it
        if (!interface_exists(iface_name)) {
            _interfaces.emplace(std::make_pair(iface_name, interfaces_create(iface_name)));
        }

        _interfaces[iface_name]->load(options);
    }
}

void Proxy::interfaces_reload(Holder managed_interfaces) {
    std::scoped_lock lock(_interface_access_mutex);
    for (auto& [iface_name, interface] : _interfaces) {
        interface->unload();
    }

    interfaces_load(managed_interfaces);
}

void Proxy::interfaces_unload(SimpleDBus::Holder removed_interfaces) {
    std::scoped_lock lock(_interface_access_mutex);
    for (auto& option : removed_interfaces.get_array()) {
        std::string iface_name = option.get_string();
        if (interface_exists(iface_name)) {
            _interfaces[iface_name]->unload();
        }
    }
}

bool Proxy::interfaces_loaded() {
    std::scoped_lock lock(_interface_access_mutex);
    for (auto& [iface_name, interface] : _interfaces) {
        if (interface->is_loaded()) {
            return true;
        }
    }
    return false;
}

// ----- CHILD HANDLING -----

bool Proxy::path_exists(const std::string& path) {
    std::scoped_lock lock(_child_access_mutex);
    return _children.find(path) != _children.end();
}

std::shared_ptr<Proxy> Proxy::path_get(const std::string& path) {
    std::scoped_lock lock(_child_access_mutex);
    if (!path_exists(path)) {
        throw Exception::PathNotFoundException(_path, path);
    }
    return _children[path];
}

void Proxy::path_add(const std::string& path, SimpleDBus::Holder managed_interfaces) {
    // If the path is not a child of the current path, then we can't add it.
    if (!Path::is_descendant(_path, path)) {
        // TODO: Should an exception be thrown here?
        return;
    }

    // If the path is already in the map, perform a reload of all interfaces.
    if (path_exists(path)) {
        path_get(path)->interfaces_load(managed_interfaces);
        return;
    }

    // As children will be extensively accessed, we need to lock the child access mutex.
    std::scoped_lock lock(_child_access_mutex);

    if (Path::is_child(_path, path)) {
        // If the path is a direct child of the proxy path, create a new proxy for it.
        std::shared_ptr<Proxy> child = path_create(path);
        child->interfaces_load(managed_interfaces);
        child->_parent = shared_from_this();
        _children.emplace(std::make_pair(path, child));
        on_child_created(path);
    } else {
        // If the new path is for a descendant of the current proxy, check if there is a child proxy for it.
        auto child_result = std::find_if(
            _children.begin(), _children.end(),
            [path](const std::pair<std::string, std::shared_ptr<Proxy>>& child_data) -> bool {
                return Path::is_descendant(child_data.first, path);
            });

        if (child_result != _children.end()) {
            // If there is a child proxy for the new path, forward it to that child proxy.
            child_result->second->path_add(path, managed_interfaces);
        } else {
            // If there is no child proxy for the new path, create the child and forward the path to it.
            // This path will be taken if an empty proxy object needs to be created for an intermediate path.
            std::string child_path = Path::next_child(_path, path);
            std::shared_ptr<Proxy> child = path_create(child_path);
            _children.emplace(std::make_pair(child_path, child));
            child->path_add(path, managed_interfaces);
            child->_parent = shared_from_this();
            on_child_created(child_path);
        }
    }
}

bool Proxy::path_remove(const std::string& path, SimpleDBus::Holder options) {
    // `options` contains an array of strings of the interfaces that need to be removed.

    if (path == _path) {
        invalidate();
        interfaces_unload(options);
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
        bool must_erase = _children.at(child_path)->path_remove(path, options);

        // if the child proxy is no longer needed and there is only one active instance of the child proxy,
        // then remove it.
        if (must_erase && _children.at(child_path).use_count() == 1) {
            _children.erase(child_path);
        }
    }

    return false;
}

bool Proxy::path_prune() {
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

Holder Proxy::path_collect() {
    SimpleDBus::Holder result = SimpleDBus::Holder::create_dict();
    SimpleDBus::Holder interfaces = SimpleDBus::Holder::create_dict();

    for (const auto& [interface_name, interface_ptr] : _interfaces) {
        SimpleDBus::Holder properties = interface_ptr->property_collect();
        interfaces.dict_append(SimpleDBus::Holder::Type::STRING, interface_name, std::move(properties));
    }

    if (!interfaces.get_dict_string().empty()) {
        result.dict_append(SimpleDBus::Holder::Type::OBJ_PATH, _path, std::move(interfaces));
    }

    for (const auto& [child_path, child] : _children) {
        SimpleDBus::Holder child_result = child->path_collect();
        // Merge child_result into result
        for (auto&& [path, child_interfaces] : child_result.get_dict_object_path()) {
            result.dict_append(SimpleDBus::Holder::Type::OBJ_PATH, std::move(path), std::move(child_interfaces));
        }
    }
    return std::move(result);
}

// ----- MANUAL CHILD HANDLING -----

void Proxy::path_append_child(const std::string& path, std::shared_ptr<Proxy> child) {
    // ! This function is used to manually add children to the proxy.

    // If the provided path is not a child of the current path, return silently.
    if (!Path::is_child(_path, path)) {
        // TODO: Should an exception be thrown here?
        return;
    }

    // As children will be extensively accessed, we need to lock the child access mutex.
    std::scoped_lock lock(_child_access_mutex);
    _children.emplace(std::make_pair(path, child));
}

void Proxy::path_remove_child(const std::string& path) {
    // ! This function is used to manually add children to the proxy.

    // If the provided path is not a child of the current path, return silently.
    if (!Path::is_child(_path, path)) {
        // TODO: Should an exception be thrown here?
        return;
    }

    std::scoped_lock lock(_child_access_mutex);
    _children.erase(path);
}

// ----- MESSAGE HANDLING -----

void Proxy::message_handle(Message& msg) {
   if (interface_exists(msg.get_interface())) {
        interface_get(msg.get_interface())->message_handle(msg);
    }

    if (msg.get_type() == Message::Type::SIGNAL) {
        auto parent = _parent.lock();
        if (parent != nullptr) {
            parent->on_child_signal_received(_path);
        }
    }
}
