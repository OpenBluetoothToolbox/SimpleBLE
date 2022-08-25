#pragma once

#include <simpledbus/Export.h>
#include <stdexcept>
#include <string>

namespace SimpleDBus {

namespace Exception {

class SIMPLEDBUS_EXPORT BaseException : public std::exception {};

class SIMPLEDBUS_EXPORT NotInitialized : public BaseException {
  public:
    NotInitialized();
    const char* what() const noexcept override;
};

class SIMPLEDBUS_EXPORT DBusException : public BaseException {
  public:
    DBusException(const std::string& err_name, const std::string& err_message);
    const char* what() const noexcept override;

  private:
    std::string _message;
};

class SIMPLEDBUS_EXPORT SendFailed : public BaseException {
  public:
    SendFailed(const std::string& err_name, const std::string& err_message, const std::string& msg_str);
    const char* what() const noexcept override;

  private:
    std::string _message;
};

class SIMPLEDBUS_EXPORT InterfaceNotFoundException : public BaseException {
  public:
    InterfaceNotFoundException(const std::string& path, const std::string& interface);
    const char* what() const noexcept override;

  private:
    std::string _message;
};

class SIMPLEDBUS_EXPORT PathNotFoundException : public BaseException {
  public:
    PathNotFoundException(const std::string& path, const std::string& subpath);
    const char* what() const noexcept override;

  private:
    std::string _message;
};

}  // namespace Exception

}  // namespace SimpleDBus
