#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "../ClassRegistry.h"
#include "BackendBase.h"

namespace SimpleBLE {

using BackendConstructor = std::function<std::shared_ptr<BackendBase>(void)>;

/**
 * Registry of all available backends.
 */
class BackendRegistry : public internal::ClassRegistry<std::vector<BackendConstructor>, BackendRegistry> {
  public:
    /**
     * Get all available backends.
     *
     * Instantiate each backend class and return a vector of pointers to them.
     * There is no provision to destroy the Backend object. This means backends
     * must be singletons and will last until the end of the program.
     */
    static std::vector<std::shared_ptr<BackendBase>> get_backends() {
        std::vector<std::shared_ptr<BackendBase>> backends;
        for (auto& cls : classes()) {
            backends.push_back(cls());
        }
        return backends;
    }

    /**
     * Get all available backends, but do not throw exceptions.
     *
     * Backend classes that fail to instantiate will be skipped.
     */
    static std::vector<std::shared_ptr<BackendBase>> get_backends_safe() noexcept {
        std::vector<std::shared_ptr<BackendBase>> backends;
        for (auto& cls : classes()) {
            try {
                backends.push_back(cls());
            } catch (...) {
                // Skip this backend
                // TODO: log error??
            }
        }
        return backends;
    }

  protected:
    template <typename T>
    static void registerCls() {
        _classes().push_back([]() { return T::get(); });
    }
};

/**
 * Base class for automatic registration of backends.
 *
 * REQUIRES: A static method `T* get()` that returns a pointer to the backend must
 * be defined.
 *
 * In practice, subclasses will be singletons. For that, use BackendSingleton.
 */
template <typename T>
class RegisteredBackend : public BackendBase, public internal::WithRegistrator<BackendRegistry, T> {};

/**
 * Convenience class to have a Backend that is a singleton.
 *
 * REQUIRES: T must be default-constructible.
 */
template <typename T>
class BackendSingleton : public RegisteredBackend<T> {
  public:
    static std::shared_ptr<T> get() {
        static T instance;
        return std::shared_ptr<T>(&instance, [](T*) {});
    }

    BackendSingleton(const BackendSingleton& other) = delete;  // Remove the copy constructor
    T& operator=(const T&) = delete;                           // Remove the copy assignment

  protected:
    BackendSingleton() = default;
};

}  // namespace SimpleBLE
