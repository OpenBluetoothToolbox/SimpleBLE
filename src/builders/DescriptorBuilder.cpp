#include "DescriptorBuilder.h"

using namespace SimpleBLE;

DescriptorBuilder::DescriptorBuilder(std::shared_ptr<DescriptorBase> internal) { internal_ = internal; }