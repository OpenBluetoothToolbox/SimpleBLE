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
    std::string path() const;

  protected:
    bool _valid;

    std::string _path;
    std::string _bus_name;
    std::shared_ptr<Connection> _conn;
};

}  // namespace SimpleDBus