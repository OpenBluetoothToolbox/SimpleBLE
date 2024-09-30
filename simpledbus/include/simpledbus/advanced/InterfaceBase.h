#pragma once

#include <simpledbus/base/Connection.h>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

class InterfaceBase {
  public:
    InterfaceBase(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path,
              const std::string& interface_name);

    virtual ~InterfaceBase() = default;

    virtual void message_handle(Message& msg) = 0;

  protected:
    std::string _path;
    std::string _bus_name;
    std::string _interface_name;
    std::shared_ptr<Connection> _conn;
};

}  // namespace SimpleDBus
