#pragma once

#include <stdexcept>

namespace SimpleBLE {

namespace Exception {

class InvalidReference : public std::runtime_error {
  public:
    InvalidReference();
};

}  // namespace Exception

}  // namespace SimpleBLE