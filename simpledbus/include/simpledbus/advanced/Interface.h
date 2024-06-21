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

    /**
     * @brief Property class is meant to facilitate using setters and getters
     *  for dbus properties. It is meant to be used to delcare properties on
     *  classes inheriting from Interface. 
     * 
     * Usage example:
     * 
     * class MyInterface : public SimpleDBus::Interface {
     * public:
     *   MyInterface(std::shared_ptr<SimpleDBus::Connection> conn, std::string destination, std::string path)
     *     : SimpleDBus::Interface(std::move(conn), std::move(destination), std::move(path)) {
     *   }
     *
     *   // Declare a property
     *   SimpleDBus::Interface::Property<bool> my_property = create_property<bool>("MyProperty");
     *
     *   // Set a property
     *   void set_my_property(bool value) {
     *     my_property.set(value);
     *   }
     *
     *   // Get a property
     *   bool get_my_property() {
     *     return my_property.get();
     *   }
     * };
     * 
     */
    template<typename T>
    class Property {
      public:
        Property(Interface& interface, const std::string& name);
        T get(bool refresh = true);
        void set(T value);

      private:
        Interface& _interface; 
        const std::string& _name; 
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

