#pragma once

#include <simplebluez/ProxyRoot.h>
#include <simpledbus/base/Connection.h>

namespace SimpleBluez {

class Bluez {
  public:

    // Note: This class MUST be consumed as a shared_ptr, as internally objects
    // are linked together and proper usage of `shared_from_this()` is relied upon.
    static std::shared_ptr<Bluez> create() {
        static std::shared_ptr<Bluez> instance = std::shared_ptr<Bluez>(new Bluez());
        instance->init();
        return instance;
    }
    virtual ~Bluez();

    // Delete copy and move operations
    Bluez(const Bluez&) = delete;
    Bluez& operator=(const Bluez&) = delete;
    Bluez(Bluez&&) = delete;
    Bluez& operator=(Bluez&&) = delete;

    void run_async();

    std::vector<std::shared_ptr<Adapter>> get_adapters();
    std::shared_ptr<Agent> get_agent();
    void register_agent();

  private:

    Bluez();
    void init();

    std::shared_ptr<SimpleDBus::Connection> _conn;
    std::shared_ptr<SimpleBluez::ProxyRoot> _bluez_root;
};

}  // namespace SimpleBluez
