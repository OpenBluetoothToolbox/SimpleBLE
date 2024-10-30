#include <simpleble/AdapterSafe.h>
#include <simpleble/BackendSafe.h>
#include <vector>

#include "BackendRegistry.h"
#include "BuildVec.h"
#include "simpleble/Backend.h"

using namespace SimpleBLE::Safe;
using UBackend = SimpleBLE::Backend;

std::optional<std::vector<Adapter>> Backend::get_adapters() noexcept {
    try {
        return (std::vector<Adapter>)build_vec(internal_.get_adapters());
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<bool> Backend::bluetooth_enabled() noexcept {
    try {
        return internal_.bluetooth_enabled();
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<bool> Backend::any_bluetooth_enabled() noexcept {
    auto backends = get_backends();
    if (!backends) {
        return std::nullopt;
    }

    try {
        for (auto& backend : *backends) {
            if (backend.bluetooth_enabled()) {
                return true;
            }
        }

        return false;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<Backend> Backend::first_bluetooth_enabled() noexcept {
    auto backends = get_backends();
    if (!backends) {
        return std::nullopt;
    }

    try {
        for (auto& backend : *backends) {
            if (backend.bluetooth_enabled()) {
                return backend;
            }
        }

        return std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

std::string Backend::backend_name() const noexcept { return internal_.backend_name(); }

std::optional<std::vector<Backend>> Backend::get_backends() noexcept {
    auto backends = BackendRegistry::get_backends_safe();
    if (backends.empty()) {
        return std::nullopt;
    }
    std::vector<Backend> safe_backends;
    for (auto& backend : backends) {
        safe_backends.push_back(build(build(backend)));
    }
    return safe_backends;
}
