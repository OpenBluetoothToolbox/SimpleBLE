#pragma once

#include <simplebluez/Bluez.h>
#include <atomic>
#include <thread>
#include <memory>

namespace SimpleBLE {

class Bluez {
  public:
    static Bluez* get();

    std::shared_ptr<SimpleBluez::Bluez> bluez;

  private:
    Bluez();
    ~Bluez();
    Bluez(Bluez& other) = delete;           // Remove the copy constructor
    void operator=(const Bluez&) = delete;  // Remove the copy assignment

    std::thread* async_thread;
    std::atomic_bool async_thread_active;
    void async_thread_function();
};

}  // namespace SimpleBLE
