#pragma once

#include <simpledbus/advanced/InterfaceBase.h>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

class LocalInterface : public InterfaceBase {
  public:
    LocalInterface(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path,
              const std::string& interface_name);

    virtual ~LocalInterface() = default;

    // ----- LIFE CYCLE -----

    // ----- METHODS -----
    Message create_method_call(const std::string& method_name);

    // ----- PROPERTIES -----
    Holder property_get(const std::string& property_name);
    void property_set(const std::string& property_name, const Holder& value);

    // ----- SIGNALS -----

    // ----- MESSAGES -----
    virtual void message_handle(Message& msg);

  protected:
    std::recursive_mutex _property_update_mutex;
    std::map<std::string, bool> _property_valid_map;
    std::map<std::string, Holder> _properties;
};

}  // namespace SimpleDBus
