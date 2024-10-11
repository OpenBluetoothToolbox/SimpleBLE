#include <simplebluez/Bluez.h>

#include <atomic>
#include <chrono>
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

void millisecond_delay(int ms) {
    for (int i = 0; i < ms; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(int argc, char* argv[]) {
    int selection = -1;

    std::thread* async_thread = new std::thread(async_thread_function);

    auto adapters = bluez->get_adapters();
    std::cout << "The following adapters were found:" << std::endl;
    for (int i = 0; i < adapters.size(); i++) {
        std::cout << "[" << i << "] " << adapters[i]->identifier() << " [" << adapters[i]->address() << "]"
                  << std::endl;
    }

    // std::cout << "Please select an adapter to advertise: ";
    // std::cin >> selection;
    // if (selection < 0 || selection >= adapters.size()) {
    //     std::cout << "Invalid selection" << std::endl;
    //     return 1;
    // }

    auto adapter = adapters[0];
    std::cout << "Advertising on " << adapter->identifier() << " [" << adapter->address() << "]" << std::endl;

    auto service_manager = bluez->get_custom_service_manager();

    auto service = service_manager->create_service();
    service->uuid("12345678-1234-5678-1234-567812345678");
    service->primary(true);

    auto characteristic = service->create_characteristic();
    characteristic->uuid("12345678-AAAA-5678-1234-567812345678");
    characteristic->flags({"read", "notify", "write"});

    adapter->register_application(service_manager->path());

    auto advertisement_manager = bluez->get_custom_advertisement_manager();
    auto advertisement = advertisement_manager->create_advertisement("potato");

    std::map<uint16_t, std::vector<uint8_t>> data;
    data[0x1024] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    advertisement->manufacturer_data(data);
    advertisement->timeout(180);
    advertisement->local_name("SimpleBluez");

    adapter->register_advertisement(advertisement->path());

    // Sleep for a bit to allow the adapter to stop discovering.
    millisecond_delay(15000);

    adapter->unregister_advertisement(advertisement->path());
    adapter->unregister_application(service_manager->path());

    millisecond_delay(5000);

    async_thread_active = false;
    while (!async_thread->joinable()) {
        millisecond_delay(10);
    }
    async_thread->join();
    delete async_thread;

    return 0;
}