#pragma once

#include <jni.h>

#include "VM.hpp"

namespace SimpleBLE {
namespace JNI {

template <typename T>
class GlobalRef {
  public:
    GlobalRef() = default;

    GlobalRef(T obj) { _obj = (T) VM::env()->NewGlobalRef(obj); }

    GlobalRef(const GlobalRef& other) {
        // Custom copy constructor
        _obj = (T) VM::env()->NewGlobalRef(other._obj);
    }

    GlobalRef& operator=(const GlobalRef& other) {
        // Custom copy assignment
        if (this != &other) {
            if (_obj != nullptr) {
                VM::env()->DeleteGlobalRef(_obj);
            }
            _obj = (T) VM::env()->NewGlobalRef(other._obj);
        }
        return *this;
    }

    ~GlobalRef() {
        if (_obj != nullptr) {
            VM::env()->DeleteGlobalRef(_obj);
        }
    }

    T* operator->() { return &_obj; }

    T get() { return _obj; }

  private:
    T _obj = nullptr;
};

}  // namespace JNI
}  // namespace SimpleBLE