#pragma once

#include <simpledbus/advanced/Interface.h>

#include <functional>

namespace SimpleDBus {

class Properties : public Interface {
  public:
    Properties(std::shared_ptr<Connection> conn, std::shared_ptr<Proxy> proxy);
    virtual ~Properties() = default;

    Holder Get(const std::string& interface_name, const std::string& property_name);
    Holder GetAll(const std::string& interface_name);
    void Set(const std::string& interface_name, const std::string& property_name, const Holder& value);

    void message_handle(Message& msg) override;
};

}  // namespace SimpleDBus
