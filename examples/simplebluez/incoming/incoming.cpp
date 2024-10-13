#include <simplebluez/Bluez.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>

std::shared_ptr<SimpleBluez::Bluez> bluez = SimpleBluez::Bluez::create();

std::atomic_bool async_thread_active = true;
void async_thread_function() {
    while (async_thread_active) {
        bluez->run_async();
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

std::atomic_bool app_running = true;
void signal_handler(int signal) { app_running = false; }

void millisecond_delay(int ms) {
    for (int i = 0; i < ms; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, signal_handler);
    std::thread* async_thread = new std::thread(async_thread_function);
    auto adapter = bluez->get_adapters()[0];

    std::cout << "Initializing SimpleBluez Peripheral Mode Demo" << std::endl;
    auto service_manager = bluez->get_custom_service_manager();
    auto advertisement_manager = bluez->get_custom_advertisement_manager();

    // --- ADAPTER SETUP ---

    // TODO: Set up all relevant adapter properties and callbacks.

    // --- SERVICE DEFINITION ---
    auto service0 = service_manager->create_service();
    service0->uuid("12345678-1234-5678-1234-567812345678");
    service0->primary(true);

    auto characteristic0 = service0->create_characteristic();
    characteristic0->uuid("12345678-AAAA-5678-1234-567812345678");
    characteristic0->flags({"read", "notify", "write"});
    // NOTE: Setting an initial value is not required, as this value doesn't get sent
    // to the central until it attempts to read or notify.

    // Register the services and characteristics.
    adapter->register_application(service_manager->path());

    // NOTE: This long delay is not necessary. However, once an application is registered
    // you want to wait until all services have been added to the adapter. This is done by
    // checking the UUIDs property of org.bluez.Adapter1.
    millisecond_delay(1000);

    // --- ADVERTISEMENT DEFINITION ---

    auto advertisement = advertisement_manager->create_advertisement("potato");
    std::map<uint16_t, std::vector<uint8_t>> data;
    data[0x1024] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    advertisement->manufacturer_data(data);
    advertisement->timeout(180);
    advertisement->local_name("SimpleBluez");

    // --- MAIN EVENT LOOP ---

    // NOTE: Right now we're registering the advertisement prior to entering the main loop,
    // but will be better handled if we figure out a way of knowing if the advertisement is
    // still active or not.

    adapter->register_advertisement(advertisement->path());
    std::cout << "Advertising on " << adapter->identifier() << " [" << adapter->address() << "]" << std::endl;

    while (app_running) {
        // TODO: Handle advertising state.

        // TODO: Handle connection events.

        // TODO: Handle data updates.
        static int value = 0;
        characteristic0->value({(uint8_t)(value), (uint8_t)(value + 1), (uint8_t)(value + 2)});
        value = (value * 1103515245 + 12345) & 0xFFFFFF;

        // This should eventually become a yield.
        millisecond_delay(1000);
    }

    // --- CLEANUP ---

    // TODO: This section should also handle any lingering device connections.

    adapter->unregister_advertisement(advertisement->path());
    adapter->unregister_application(service_manager->path());

    async_thread_active = false;
    async_thread->join();
    delete async_thread;

    return 0;
}
