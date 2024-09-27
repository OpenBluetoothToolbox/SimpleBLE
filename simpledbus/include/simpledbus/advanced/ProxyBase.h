#pragma once

#include <simpledbus/advanced/InterfaceBase.h>
#include <simpledbus/external/kvn_safe_callback.hpp>
#include <simpledbus/base/Path.h>

#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

class InterfaceBase;

class ProxyBase : public std::enable_shared_from_this<ProxyBase> {
  public:
    ProxyBase(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~ProxyBase();

    bool valid() const;
    std::string path() const;


    const std::map<std::string, std::shared_ptr<ProxyBase>>& children();
    const std::map<std::string, std::shared_ptr<InterfaceBase>>& interfaces();

    virtual std::shared_ptr<InterfaceBase> interfaces_create(const std::string& name);
    virtual std::shared_ptr<ProxyBase> path_create(const std::string& path);

    // ----- INTERFACE HANDLING -----
    std::shared_ptr<InterfaceBase> interface_get(const std::string& name);
    bool interface_exists(const std::string& name);
    void interface_insert(const std::string& name);
    void interface_remove(const std::string& name);
    bool interfaces_loaded();

    // ----- CHILD HANDLING -----
    std::shared_ptr<ProxyBase> path_get(const std::string& path);
    bool path_exists(const std::string& path);
    bool path_prune();
    void path_append_child(const std::string& path, std::shared_ptr<ProxyBase> child);

    // ----- MESSAGE HANDLING -----
    void message_forward(Message& msg);
    virtual void message_handle(Message& msg);

    // ----- CALLBACKS -----

    // ----- TEMPLATE METHODS -----
    template <typename T>
    std::vector<std::shared_ptr<T>> children_casted() {
        std::vector<std::shared_ptr<T>> result;
        std::scoped_lock lock(_child_access_mutex);
        for (auto& [path, child] : _children) {
            result.push_back(std::dynamic_pointer_cast<T>(child));
        }
        return result;
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> children_casted_with_prefix(const std::string& prefix) {
        std::vector<std::shared_ptr<T>> result;
        std::scoped_lock lock(_child_access_mutex);
        for (auto& [path, child] : _children) {
            const std::string next_child = SimpleDBus::Path::next_child_strip(_path, path);
            if (next_child.find(prefix) == 0) {
                result.push_back(std::dynamic_pointer_cast<T>(child));
            }
        }
        return result;
    }

  protected:
    bool _valid;
    std::string _path;
    std::string _bus_name;

    std::shared_ptr<Connection> _conn;

    std::map<std::string, std::shared_ptr<InterfaceBase>> _interfaces;
    std::map<std::string, std::shared_ptr<ProxyBase>> _children;

    std::recursive_mutex _interface_access_mutex;
    std::recursive_mutex _child_access_mutex;
};

}  // namespace SimpleDBus
