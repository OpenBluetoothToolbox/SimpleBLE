#include "Bluez.h"
#include <mutex>
#include <unistd.h> // Linux-specific header for fork-related stuff
#include <iostream> // TODO: this is only for temporary error handling


using namespace SimpleBLE;

Bluez* Bluez::get() {
    // This mutex does not actually ensure thread-safety. The lock constructed from it is destroyed
    // immediately after returning from this function, so the mutex is unlocked before the returned pointer
    // is accessed.
    // Additionally, the mutex is unnecessary to guard the initialization of instance, because it is
    // thread-safe anyway (see [stmt.dcl#3] in the standard). And taking the address of instance 
    // does not require a lock.

    // static std::mutex get_mutex;       // Static mutex to ensure thread safety when accessing the logger
    // std::scoped_lock lock(get_mutex);  // Unlock the mutex on function return

    // Static instance of the logger to ensure proper lifecycle management
    static Bluez instance = []() -> Bluez { 
	// Setting the fork handlers must be done here and not in the constructor of Bluez, because then
	// Bluez::get() would have to be called from the constructor while instance is being initialized, 
	// and that kind of recursion is UB [stmt.dcl#3].

	// Remove old fork handlers, if any
	// TODO: handle error better
	if(pthread_atfork(nullptr, nullptr, nullptr)) std::cout << "pthread_atfork() error" << std::endl;

	// Set the fork handlers
	// TODO: handle error better
	if(pthread_atfork(
	    // This is called in the parent before fork():
	    +[]() -> void { instance.stop_async_thread(); },
	    // This is called in the parent after fork() completes:
	    +[]() -> void { instance.start_async_thread(); },
	    // This is called in the child after fork() completes:
	    +[]() -> void { instance.start_async_thread(); }
	)) std::cout << "pthread_atfork() error" << std::endl;
	
	// We're returning a pr-value, all copies are elided in this initialization
	return {};
    }();

    return &instance;
}


Bluez::Bluez() {


    bluez.init();
    start_async_thread();
}

Bluez::~Bluez() {
    // std::thread::joinable() does not tell us whether the thread has finished execution, it will always
    // return true unless std::thread::detach() or std::thread::join() has been called
    //
    // But there is no need to wait in a loop anyway, since std::thread::join() blocks the current thread
    // until the target thread completes

    // while (!async_thread->joinable()) {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // }

    // delete async_thread;
    
    stop_async_thread();
}

void Bluez::async_thread_function() {
    while (async_thread_active.load(std::memory_order_acquire)) {
        bluez.run_async();
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void Bluez::start_async_thread() {
    async_thread_active.store(true, std::memory_order_relaxed);
    async_thread = std::thread{&Bluez::async_thread_function, this};
}

void Bluez::stop_async_thread() {
    async_thread_active.store(false, std::memory_order_release);
    async_thread.join();
}

void Bluez::set_fork_handlers() {
}
