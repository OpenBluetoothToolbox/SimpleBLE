#pragma once

#include <simpledbus/base/Connection.h>
#include <simplebluez/Types.h>

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
        Property(Interface& interface, std::string name);
        virtual T get();
        virtual T refresh_and_get();
        virtual void set(T value);

      protected:
        Interface& _interface; 
        std::string _name; 
    };

    template<typename K>
    class Property<std::vector<K>> {
      public:
        Property(Interface& interface, std::string name);

        virtual std::vector<K> get(); 
        virtual std::vector<K> refresh_and_get();

      protected:
        Interface& _interface; 
        std::string _name; 
    };


    template<typename K, typename V>
    class Property<std::map<K, std::vector<V>>> {
      public:
        Property(Interface& interface, std::string name) : _interface(interface), _name(name) {}

        virtual std::map<K, std::vector<V>> get() {
          std::scoped_lock lock(_interface._property_update_mutex);
          std::map<K, std::vector<V>> propmap;
          std::map<K, SimpleDBus::Holder> prop_map = _interface._properties[_name].get<std::map<K, SimpleDBus::Holder>>();
          // Loop through all received keys and store them.
          for (auto& [key, value_array] : prop_map) {
              std::vector<V> raw_service_data;
              for (SimpleDBus::Holder& elem : value_array.get_array()) {
                  raw_service_data.push_back(elem.get<V>());
              }
              propmap[key] = raw_service_data;
          }
          return propmap;
        }

        virtual std::map<K, std::vector<V>> refresh_and_get() {
          _interface.property_refresh(_name);
          return get();
        }

      protected:
        Interface& _interface; 
        std::string _name; 
    };


    template<typename T>
    class CachedProperty : public Property<T> {
      public:
        CachedProperty(Interface& interface, std::string name);
        T get() override;
        T refresh_and_get() override;
        void update_cached_property(); 

      private:
        T _cached_property = T();
    };

    Interface(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path,
              const std::string& interface_name);

    virtual ~Interface() = default;

    // ----- LIFE CYCLE -----
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

