#pragma once

#include <simpledbus/base/Connection.h>
#include <simpledbus/advanced/ProxyBase.h>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

class ProxyBase;

class InterfaceBase  {
  public:
    InterfaceBase(std::shared_ptr<Connection> conn, std::shared_ptr<ProxyBase> proxy,
                  const std::string& interface_name);

    virtual ~InterfaceBase() = default;

    // ----- METHODS -----

    // ----- PROPERTIES -----

    // ----- SIGNALS -----

    // ----- MESSAGES -----
    virtual void message_handle(Message& msg);

  protected:
    std::string _interface_name;
    std::weak_ptr<ProxyBase> _proxy;
    std::shared_ptr<Connection> _conn;
};

}  // namespace SimpleDBus
