#pragma once

#include <simpledbus/advanced/Interface.h>
#include <simpledbus/advanced/ProxyBase.h>
#include <simpledbus/external/kvn_safe_callback.hpp>
#include <simpledbus/base/Path.h>

#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

class Proxy : public ProxyBase {
  public:
    Proxy(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~Proxy();

    // ----- INTROSPECTION -----
    std::string introspect();

    // ----- INTERFACE HANDLING -----
    void interfaces_load(Holder managed_interfaces);
    void interfaces_unload(Holder removed_interfaces);

    // ----- CHILD HANDLING -----
    void path_add(const std::string& path, Holder managed_interfaces);
    bool path_remove(const std::string& path, Holder removed_interfaces);
    
    // ----- MESSAGE HANDLING -----
    virtual void message_handle(Message& msg);
    void message_forward(Message& msg);

    // ----- CALLBACKS -----
    kvn::safe_callback<void(std::string)> on_child_created;
    kvn::safe_callback<void(std::string)> on_child_signal_received;

    // ----- TEMPLATE METHODS -----

  protected:
    SimpleDBus::Holder _collect_managed_objects(const std::string& base_path);
};

}  // namespace SimpleDBus
