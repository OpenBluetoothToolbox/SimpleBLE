#pragma once

#include <simpledbus/base/Connection.h>
#include <simplebluez/Types.h>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

// template<typename T>
// class Property {
//   public:
//     Property(Interface& interface, std::string name);
//     virtual T get();
//     virtual T refresh_and_get();
//     virtual void set(T value);

//   protected:
//     Interface& _interface; 
//     std::string _name; 
// };

// template<typename K>
// class Property<std::vector<K>> {
//   public:
//     Property(Interface& interface, std::string name);

//     virtual std::vector<K> get(); 
//     virtual std::vector<K> refresh_and_get();

//   protected:
//     Interface& _interface; 
//     std::string _name; 
// };


// template<typename K, typename V>
// class Property<std::map<K, std::vector<V>>> {
//   public:
//     Property(Interface& interface, std::string name);
//     virtual std::map<K, std::vector<V>> get();
//     virtual std::map<K, std::vector<V>> refresh_and_get(); 

//   protected:
//     Interface& _interface; 
//     std::string _name; 
// };


// template<typename T>
// class CachedProperty : public Property<T> {
//   public:
//     CachedProperty(Interface& interface, std::string name);
//     T get() override;
//     T refresh_and_get() override;
//     void update_cached_property(); 

//   private:
//     T _cached_property = T();
// };

// template<typename K, typename T>
// class CachedProperty<std::map<K, std::vector<T>>> : public Property<std::map<K, std::vector<T>>> {
//   public:
//     CachedProperty(Interface& interface, std::string name);
//     std::map<K, std::vector<T>> get() override;
//     std::map<K, std::vector<T>> refresh_and_get() override;
//     void update_cached_property();
  
//   private:
//     std::map<K, std::vector<T>> _cached_property;
// };


class Interface {
  public:

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

    template<typename T>
    friend class Property;

};

}  // namespace SimpleDBus

