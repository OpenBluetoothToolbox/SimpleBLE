#pragma once

#include <simpledbus/advanced/LocalInterface.h>
#include <simpledbus/advanced/ProxyBase.h>
#include <simpledbus/external/kvn_safe_callback.hpp>
#include <simpledbus/base/Path.h>

#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

class LocalProxy : public ProxyBase {
  public:
    LocalProxy(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~LocalProxy();

    bool path_exists(const std::string& path);
    std::shared_ptr<LocalProxy> path_get(const std::string& path);

    bool interface_exists(const std::string& name);
    std::shared_ptr<LocalInterface> interface_get(const std::string& name);

    const std::map<std::string, std::shared_ptr<LocalProxy>>& children();
    const std::map<std::string, std::shared_ptr<LocalInterface>>& interfaces();

    virtual std::shared_ptr<LocalInterface> interfaces_create(const std::string& name);
    virtual std::shared_ptr<LocalProxy> path_create(const std::string& path);

    // ----- INTROSPECTION -----
    std::string introspect();

    // ----- INTERFACE HANDLING -----
    void interfaces_load(const std::string& interface_name, std::shared_ptr<LocalInterface> interface);
    void interfaces_unload(const std::string& interface_name);

    // ----- CHILD HANDLING -----
    void path_add(const std::string& path, std::shared_ptr<LocalProxy> child);
    bool path_remove(const std::string& path);
    bool path_prune();

    // ----- MESSAGE HANDLING -----
    virtual void message_handle(Message& msg);
    void message_forward(Message& msg);

    // ----- CALLBACKS -----
    kvn::safe_callback<void(std::string)> on_child_created;
    kvn::safe_callback<void(std::string)> on_child_signal_received;

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
    std::map<std::string, std::shared_ptr<LocalInterface>> _interfaces;
    std::map<std::string, std::shared_ptr<LocalProxy>> _children;

    std::recursive_mutex _interface_access_mutex;
    std::recursive_mutex _child_access_mutex;
};

}  // namespace SimpleDBus