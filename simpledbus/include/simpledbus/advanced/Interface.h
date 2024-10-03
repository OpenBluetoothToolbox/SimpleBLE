#pragma once

#include <simpledbus/advanced/Proxy.h>
#include <simpledbus/base/Connection.h>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

class Proxy;

class Interface {
  public:
    Interface(std::shared_ptr<Connection> conn, Proxy* proxy, const std::string& interface_name);

    virtual ~Interface() = default;

    // ----- LIFE CYCLE -----
    void load(Holder options);
    void unload();
    bool is_loaded() const;

    // ----- METHODS -----
    Message create_method_call(const std::string& method_name);

    // ----- PROPERTIES -----
    virtual void property_changed(std::string option_name);

    // ! These functions are used by the Properties interface. We need better nomenclature!
    Holder property_collect();
    Holder property_collect_single(const std::string& property_name);
    void property_modify(const std::string& property_name, const Holder& value);

    // ! TODO: We need to figure out a good architecture to let any generic interface access the Properties object of its Proxy.
    Holder property_get_all();
    Holder property_get(const std::string& property_name);
    void property_set(const std::string& property_name, const Holder& value);

    void property_refresh(const std::string& property_name);

    // ----- SIGNALS -----
    void signal_property_changed(Holder changed_properties, Holder invalidated_properties);

    // ----- MESSAGES -----
    virtual void message_handle(Message& msg);

  protected:
    std::atomic_bool _loaded{true};

    // NOTE: We should probably keep a copy of the proxy fields that are used often to avoid locking
    //       the proxy mutex for too long. Proxy should be locked only when a call is required, not to read
    //       a static property.
    Proxy* _proxy;
    std::string _interface_name;
    std::shared_ptr<Connection> _conn;

    std::recursive_mutex _property_update_mutex;
    std::map<std::string, bool> _property_valid_map;

    /**
     * @brief Dictionary containing all properties.
     *
     * @note: When accessing this object, the _property_update_mutex must be locked.
     */
    std::map<std::string, Holder> _properties;
};

}  // namespace SimpleDBus
