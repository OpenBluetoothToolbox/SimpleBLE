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

    template<typename T>
    class CachedProperty : public Property<T> {
      public:
        CachedProperty(Interface& interface, std::string name): 
          Property<T>(interface, name) {}

        T get() override {
            return this->_cached_property;
        }

        T refresh_and_get() override {
          return Property<T>::refresh_and_get();
        }

        void update_cached_property() {
            this->_cached_property = this->refresh_and_get();
        }

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

