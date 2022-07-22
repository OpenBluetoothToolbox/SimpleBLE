#include "ServiceBuilder.h"

using namespace SimpleBLE;

ServiceBuilder::ServiceBuilder(std::shared_ptr<ServiceBase> internal) { internal_ = internal; }