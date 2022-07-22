#pragma once

#include <simpleble/Service.h>
#include <memory>

namespace SimpleBLE {

/**
 * @brief Helper class to build a Service object.
 *
 * @details This class provides access to the protected properties of Service
 *          and acts as a constructor, avoiding the need to expose any unneeded
 *          functions to the user.
 *
 */
class ServiceBuilder : public Service {
  public:
    ServiceBuilder(std::shared_ptr<ServiceBase> internal);
    virtual ~ServiceBuilder() = default;
};

}  // namespace SimpleBLE
