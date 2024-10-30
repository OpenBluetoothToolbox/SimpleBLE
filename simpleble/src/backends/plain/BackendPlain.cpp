#include <string>
#include "AdapterPlain.h"
#include "BackendRegistry.h"

namespace SimpleBLE {

class BackendPlain : public BackendSingleton<BackendPlain> {
  public:
    virtual std::vector<std::shared_ptr<AdapterBase>> get_adapters() override;
    virtual bool bluetooth_enabled() override;
    std::string backend_name() const noexcept override;
};

void BACKEND_PLAIN() {};

std::string BackendPlain::backend_name() const noexcept { return "Plain"; }

bool BackendPlain::bluetooth_enabled() { return true; }

std::vector<std::shared_ptr<AdapterBase>> BackendPlain::get_adapters() {
    std::vector<std::shared_ptr<AdapterBase>> adapters;
    adapters.push_back(std::make_shared<AdapterPlain>());
    return adapters;
}

}  // namespace SimpleBLE
