#include "simpledbus/advanced/Property.h"
#include "simpledbus/advanced/Interface.h"

using namespace SimpleDBus;

template<typename T>
Property<T>::Property(Interface& interface, std::string name)
    : _interface(interface), _name(name) {}


template<typename T>
T Property<T>::get() {
    std::scoped_lock lock(_interface._property_update_mutex);
    return _interface._properties[_name].get<T>();
}

template<typename K>
Property<std::vector<K>>::Property(Interface& interface, std::string name) : _interface(interface), _name(name) {}

template<typename K>
std::vector<K> Property<std::vector<K>>::get() {
    std::scoped_lock lock(_interface._property_update_mutex);
    std::vector<K> container;
    for (SimpleDBus::Holder& item : _interface._properties[_name].get_array()) {
        container.push_back(item.get<K>());
    }
    return container;
}

template<typename K>
std::vector<K> Property<std::vector<K>>::refresh_and_get() {
    _interface.property_refresh(_name);
    return get();
}

template<typename K, typename V>
Property<std::map<K, std::vector<V>>>::Property(Interface& interface, std::string name) : _interface(interface), _name(name) {}

template<typename K, typename V>
std::map<K, std::vector<V>> Property<std::map<K, std::vector<V>>>::get() {
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

template<typename K, typename V>
std::map<K, std::vector<V>> Property<std::map<K, std::vector<V>>>::refresh_and_get() {
    _interface.property_refresh(_name);
    return get();
}



template<typename T>
T Property<T>::refresh_and_get() {
    _interface.property_refresh(_name);
    return Property<T>::get();
}

template<typename T>
void Property<T>::set(T value) {
    std::scoped_lock lock(_interface._property_update_mutex);
    _interface.property_set(_name, SimpleDBus::Holder::create<T>(value));
}


// ----- CACHED PROPERTY -----

template<typename T>
CachedProperty<T>::CachedProperty(Interface& interface, std::string name)
    : Property<T>(interface, name) {}

template<typename T>
T CachedProperty<T>::get() {
    return this->_cached_property;
}

template<typename T>
T CachedProperty<T>::refresh_and_get() {
    update_cached_property();
    return _cached_property; 
}

template<typename T>
void CachedProperty<T>::update_cached_property() {
    this->_cached_property = Property<T>::refresh_and_get();;
}

template<typename K, typename T>
CachedProperty<std::map<K, std::vector<T>>>::CachedProperty(Interface& interface, std::string name) : Property<std::map<K, std::vector<T>>>(interface, name) {}

template<typename K, typename T>
std::map<K, std::vector<T>> CachedProperty<std::map<K, std::vector<T>>>::get() {
    return this->_cached_property;
}

template<typename K, typename T>
std::map<K, std::vector<T>> CachedProperty<std::map<K, std::vector<T>>>::refresh_and_get() {
    update_cached_property();
    return _cached_property; 
}

template<typename K, typename T>
void CachedProperty<std::map<K, std::vector<T>>>::update_cached_property() {
    this->_cached_property = Property<std::map<K, std::vector<T>>>::refresh_and_get();;
}


// ---- TEMPLATE INSTANTIATIONS -----
template class Property<uint8_t>;
template class Property<uint16_t>;
template class Property<uint32_t>;
template class Property<uint64_t>;
template class Property<int8_t>;
template class Property<int16_t>;
template class Property<int32_t>;
template class Property<int64_t>;
template class Property<bool>;
template class Property<std::string>;
template class Property<std::vector<std::string>>;
template class Property<std::map<uint16_t, std::vector<uint8_t>>>;
template class Property<std::map<std::string, std::vector<uint8_t>>>;


template class CachedProperty<uint8_t>;
template class CachedProperty<uint16_t>;
template class CachedProperty<uint32_t>;
template class CachedProperty<uint64_t>;
template class CachedProperty<int8_t>;
template class CachedProperty<int16_t>;
template class CachedProperty<int32_t>;
template class CachedProperty<int64_t>;
template class CachedProperty<bool>;
template class CachedProperty<std::string>;
template class CachedProperty<std::map<uint16_t, std::vector<uint8_t>>>;
template class CachedProperty<std::map<std::string, std::vector<uint8_t>>>;