#include "Bluez.h"
#include <mutex>
#include <unistd.h> // Linux-specific header for fork-related stuff


/* 
 * Based on https://stackoverflow.com/questions/39890363/what-happens-when-a-thread-forks 
 * and https://man7.org/linux/man-pages/man3/fork.3p.html , "If a multi-
 * threaded process calls fork(), the new process shall contain
 * a replica of the calling thread". The other threads remain in some kind of "frozen" state.
 * I don't know how to wake them back up, and in order to avoid resource leaks caused by those 
 * suspended threads accumulating, it's best to stop the thread polling bluez.run_async() 
 * before forking, and restart it afterwards. 
 */

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

    static Bluez instance;             // Static instance of the logger to ensure proper lifecycle management

    return &instance;
}


Bluez::Bluez() {
    /*
     * This solution would register functions to be called each time fork() is called.
     * But since the child handler is not allowed to call any function that's not
     * async-signal-safe (man 7 signal-safety), there's no way to use the handler
     * to start a thread.
     * Additionally, async_thread would have to be made static, since there's no way
     * to pass user-data to pthread_atfork().
     */

    /*
     * Overall, this seems like a bad solution. Perhaps it would be better to write
     * our own wrapper for fork(), but that would be a change in the API of the library.
     */
       
    /*
    // Remove old fork handlers
    if(pthread_atfork(nullptr, nullptr, nullptr) != 0) ; // TODO: handle error

    // Set the fork handlers
    if(pthread_atfork(
	// This is called in the parent before fork():
	+[]() -> void { async_thread_stop(); },
	// This is called in the parent after fork() completes:
	+[]() -> void { start_async_thread(); },
	// This is called in the child after fork() completes:
	+[]() -> void { start_async_thread(); }
    ) != 0) ; // TODO: handle error
    */


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


