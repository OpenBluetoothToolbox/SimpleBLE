#pragma once

#include <simpledbus/base/Connection.h>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

class Interface {
  public:

    template<typename T>
    class Property {
      public:
        Property(Interface& interface, const std::string& name)
            : _interface(interface), _name(name) {}

        T get(bool refresh = true) {
          if (refresh) {
            _interface.property_refresh(_name);
          }
          std::scoped_lock lock(_interface._property_update_mutex);
          return _interface._properties[_name].template get<T>();
        }

        void set(T value) {
          std::scoped_lock lock(_interface._property_update_mutex);
          _interface.property_set(_name, SimpleDBus::Holder::create<T>(value));
        }
      private:
        Interface& _interface; ///< The interface object.
        const std::string& _name; ///< The name of the property.
    };

    Interface(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path,
              const std::string& interface_name);

    virtual ~Interface() = default;


    // ----- LIFE CYCLE -----
    template<typename T>
    Property<T> create_property(const std::string& name); 
    void load(Holder options);
    void unload();
    bool is_loaded() const;

    // ----- METHODS -----
    Message create_method_call(const std::string& method_name);

    // ----- PROPERTIES -----
    virtual void property_changed(std::string option_name);

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

    std::string _path;
    std::string _bus_name;
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

