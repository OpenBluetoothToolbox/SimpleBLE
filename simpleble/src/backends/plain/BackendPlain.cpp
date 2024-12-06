#include <string>
#include "AdapterPlain.h"
#include "BackendUtils.h"
#include "CommonUtils.h"

namespace SimpleBLE {

class BackendPlain : public BackendSingleton<BackendPlain> {
  public:
    BackendPlain(buildToken) {};
    virtual ~BackendPlain() = default;

    virtual vec_of_shared<AdapterBase> get_adapters() override;
    virtual bool bluetooth_enabled() override;
    std::string backend_name() const noexcept override;
};

std::shared_ptr<BackendBase> BACKEND_PLAIN() { return BackendPlain::get(); }

std::string BackendPlain::backend_name() const noexcept { return "Plain"; }

bool BackendPlain::bluetooth_enabled() { return true; }

vec_of_shared<AdapterBase> BackendPlain::get_adapters() {
    vec_of_shared<AdapterBase> adapters;
    adapters.push_back(std::make_shared<AdapterPlain>());
    return adapters;
}

}  // namespace SimpleBLE
