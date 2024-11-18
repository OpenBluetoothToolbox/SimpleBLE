#include <vector>

#include <simpleble/Adapter.h>
#include <simpleble/Exceptions.h>

#include "BackendBase.h"
#include "BuildVec.h"
#include "CommonUtils.h"

#include "Backend.h"

using namespace SimpleBLE;

namespace SimpleBLE {

#define SIMPLEBLE_ENABLE(name)                  \
    extern std::shared_ptr<BackendBase> name(); \
    if constexpr (SIMPLEBLE_##name) return name();

static std::shared_ptr<BackendBase> _get_enabled_backend(){
    SIMPLEBLE_ENABLE(BACKEND_LINUX) SIMPLEBLE_ENABLE(BACKEND_WINDOWS) SIMPLEBLE_ENABLE(BACKEND_ANDROID)
        SIMPLEBLE_ENABLE(BACKEND_MACOS) SIMPLEBLE_ENABLE(BACKEND_PLAIN)}

Backend get_enabled_backend() {
    return Factory::build(_get_enabled_backend());
}

// NOTE: in the future, this can return multiple backends
static vec_of_shared<BackendBase> _get_backends() {
    vec_of_shared<BackendBase> backends = {_get_enabled_backend()};
    return backends;
}

}  // namespace SimpleBLE

std::vector<Backend> Backend::get_backends() { return Factory::vector(_get_backends()); }

bool Backend::initialized() const { return internal_ != nullptr; }

BackendBase* Backend::operator->() {
    if (!initialized()) {
        throw Exception::NotInitialized();
    }
    return internal_.get();
}

const BackendBase* Backend::operator->() const {
    if (!initialized()) {
        throw Exception::NotInitialized();
    }
    return internal_.get();
}

std::vector<Adapter> Backend::get_adapters() { return Factory::vector((*this)->get_adapters()); }

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
