#pragma once

#include <simpledbus/base/Connection.h>
#include <simplebluez/Types.h>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

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

class BytearrayProperty {
  public:
    BytearrayProperty(Interface& interface, std::string name);
    virtual std::string get(); 
    virtual std::string refresh_and_get(); 
    virtual void update_cached_property(); 
    virtual void update_cached_property(SimpleDBus::Holder& holder); 


  protected:
    std::string _value;
    Interface& _interface; 
    std::string _name; 
};



template<typename K, typename V>
class Property<std::map<K, std::vector<V>>> {
  public:
    Property(Interface& interface, std::string name);
    virtual std::map<K, std::vector<V>> get();
    virtual std::map<K, std::vector<V>> refresh_and_get(); 

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

template<typename K, typename T>
class CachedProperty<std::map<K, std::vector<T>>> : public Property<std::map<K, std::vector<T>>> {
  public:
    CachedProperty(Interface& interface, std::string name);
    std::map<K, std::vector<T>> get() override;
    std::map<K, std::vector<T>> refresh_and_get() override;
    void update_cached_property();
  
  private:
    std::map<K, std::vector<T>> _cached_property;
};

}