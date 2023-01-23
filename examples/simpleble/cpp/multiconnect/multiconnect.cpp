#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "../common/utils.hpp"

#include "simpleble/SimpleBLE.h"

using namespace std::chrono_literals;

int main() {
    auto adapter_optional = Utils::getAdapter();

    if (!adapter_optional.has_value()) {
        return EXIT_FAILURE;
    }

    auto adapter = adapter_optional.value();

    std::vector<SimpleBLE::Peripheral> peripherals;

    adapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) { peripherals.push_back(peripheral); });

    // Scan for 5 seconds and return.
    adapter.scan_for(5000);

    std::cout << "The following devices were found:" << std::endl;
    for (size_t i = 0; i < peripherals.size(); i++) {
        std::cout << "[" << i << "] " << peripherals[i].identifier() << " [" << peripherals[i].address() << "]"
                  << std::endl;
    }

    auto selection = Utils::getUserInputInt("Please select a device to connect to", peripherals.size() - 1);
    if (!selection.has_value()) {
        return EXIT_FAILURE;
    }

    auto peripheral = peripherals[selection.value()];
    std::cout << "Connecting to " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;

    for (size_t i = 0; i < 5; i++) {
        try {
            peripheral.connect();
            std::cout << "Successfully connected." << std::endl;
            std::this_thread::sleep_for(2s);

            peripheral.disconnect();
            std::cout << "Successfully disconnected." << std::endl;
            std::this_thread::sleep_for(2s);
        } catch (const std::exception& ex) {
            std::cout << "Failed at " << i << " with: " << ex.what() << std::endl;
            throw;
        }
    }

    return EXIT_SUCCESS;
}
