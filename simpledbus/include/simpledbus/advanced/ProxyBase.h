#pragma once

#include <simpledbus/base/Connection.h>
#include <string>
#include <memory>

namespace SimpleDBus {

class ProxyBase : public std::enable_shared_from_this<ProxyBase> {
  public:
    ProxyBase(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~ProxyBase();

    bool valid() const;
    void invalidate();
    std::string path() const;

  protected:
    bool _valid;

    std::string _path;
    std::string _bus_name;
    std::shared_ptr<Connection> _conn;

    bool _registered;
    void register_object_path();
    void unregister_object_path();

    virtual void message_handle(Message& msg);
};

}  // namespace SimpleDBus