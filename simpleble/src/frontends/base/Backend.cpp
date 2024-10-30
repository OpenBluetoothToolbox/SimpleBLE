#include <vector>

#include <simpleble/Adapter.h>
#include <simpleble/Backend.h>
#include <simpleble/Exceptions.h>
#include <sys/cdefs.h>

#include "BackendRegistry.h"
#include "BuildVec.h"

using namespace SimpleBLE;

namespace SimpleBLE {

#define SIMPLEBLE_ENABLE(name) \
    extern void name();        \
    if constexpr (SIMPLEBLE_##name) name();

/**
 * Force the linker to include all backends in the final binary.
 *
 * This is necessary when using the static library, otherwise the user has to
 * use --whole-archive when linking the library.
 *
 * This works by creating a reference to a symbol in each backend, which forces
 * the linker to include the object file, which contains the backend
 * initializator object as a global variable.
 */
SIMPLEBLE_EXPORT void _ref_backends() {
    SIMPLEBLE_ENABLE(BACKEND_PLAIN)
    SIMPLEBLE_ENABLE(BACKEND_LINUX)
    SIMPLEBLE_ENABLE(BACKEND_WINDOWS)
    SIMPLEBLE_ENABLE(BACKEND_ANDROID)
    SIMPLEBLE_ENABLE(BACKEND_MACOS)
}

}  // namespace SimpleBLE

std::vector<Backend> Backend::get_backends() { return build_vec(BackendRegistry::get_backends()); }

BackendBase* Backend::operator->() {
    if (!internal_) {
        throw Exception::NotInitialized();
    }
    return internal_.get();
}

const BackendBase* Backend::operator->() const {
    if (!internal_) {
        throw Exception::NotInitialized();
    }
    return internal_.get();
}

std::vector<Adapter> Backend::get_adapters() { return build_vec((*this)->get_adapters()); }

bool Backend::bluetooth_enabled() { return (*this)->bluetooth_enabled(); }

std::optional<Backend> Backend::first_bluetooth_enabled() {
    for (auto& backend : get_backends()) {
        if (backend->bluetooth_enabled()) {
            return backend;
        }
    }
    return std::nullopt;
}

std::string Backend::backend_name() const noexcept { return (*this)->backend_name(); }
