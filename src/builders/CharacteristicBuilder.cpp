#include "CharacteristicBuilder.h"

using namespace SimpleBLE;

CharacteristicBuilder::CharacteristicBuilder(std::shared_ptr<CharacteristicBase> internal) { internal_ = internal; }