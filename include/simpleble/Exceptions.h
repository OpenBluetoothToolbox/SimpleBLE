#pragma once

#include <stdexcept>
#include "Types.h"

namespace SimpleBLE {

namespace Exception {

class InvalidReference : public std::runtime_error {
  public:
    InvalidReference();
};

class ServiceNotFound : public std::runtime_error {
  public:
    ServiceNotFound(BluetoothUUID uuid);
};

class CharacteristicNotFound : public std::runtime_error {
  public:
    CharacteristicNotFound(BluetoothUUID uuid);
};

}  // namespace Exception

}  // namespace SimpleBLE
