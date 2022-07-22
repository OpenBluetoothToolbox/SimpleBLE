#pragma once

#include <simpleble/Characteristic.h>
#include <memory>

namespace SimpleBLE {

/**
 * @brief Helper class to build a Characteristic object.
 *
 * @details This class provides access to the protected properties of Characteristic
 *          and acts as a constructor, avoiding the need to expose any unneeded
 *          functions to the user.
 *
 */
class CharacteristicBuilder : public Characteristic {
  public:
    CharacteristicBuilder(std::shared_ptr<CharacteristicBase> internal);
    virtual ~CharacteristicBuilder() = default;
};

}  // namespace SimpleBLE
