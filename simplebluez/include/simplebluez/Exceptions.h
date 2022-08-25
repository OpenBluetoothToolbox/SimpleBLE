#pragma once

#include <stdexcept>
#include <string>

#include <simplebluez/Export.h>
#include <simpledbus/base/Exceptions.h>

namespace SimpleBluez {

namespace Exception {

class SIMPLEBLUEZ_EXPORT BaseException : public std::exception {};

class SIMPLEBLUEZ_EXPORT ServiceNotFoundException : public BaseException {
  public:
    ServiceNotFoundException(const std::string& service);
    const char* what() const noexcept override;

  private:
    std::string _message;
};

class SIMPLEBLUEZ_EXPORT CharacteristicNotFoundException : public BaseException {
  public:
    CharacteristicNotFoundException(const std::string& characteristic);
    const char* what() const noexcept override;

  private:
    std::string _message;
};

class SIMPLEBLUEZ_EXPORT DescriptorNotFoundException : public BaseException {
  public:
    DescriptorNotFoundException(const std::string& descriptor);
    const char* what() const noexcept override;

  private:
    std::string _message;
};

}  // namespace Exception

}  // namespace SimpleBluez
