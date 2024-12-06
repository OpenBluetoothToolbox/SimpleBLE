#pragma once

#include "../common/BackendBase.h"
#include "../common/BackendUtils.h"

namespace SimpleBLE {

class BackendWindows : public BackendSingleton<BackendWindows> {
  public:
    BackendWindows(buildToken);
    virtual ~BackendWindows() = default;

    virtual std::vector<std::shared_ptr<AdapterBase>> get_adapters() override;
    virtual bool bluetooth_enabled() override;
    virtual std::string backend_name() const noexcept override;
};

}  // namespace SimpleBLE
