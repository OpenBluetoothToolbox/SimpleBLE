#include "simpledbus/advanced/ProxyBase.h"

using namespace SimpleDBus;

ProxyBase::ProxyBase(std::shared_ptr<Connection> conn, const std::string& bus_name, const std::string& path)
    : _conn(conn), _bus_name(bus_name), _path(path), _valid(true) {}

ProxyBase::~ProxyBase() {}

bool ProxyBase::valid() const { return _valid; }

std::string ProxyBase::path() const { return _path; }
