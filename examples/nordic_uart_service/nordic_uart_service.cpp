#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "simpleble/SimpleBLE.h"

const SimpleBLE::BluetoothUUID NUS_service("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
const SimpleBLE::BluetoothUUID NUS_tx("6e400002-b5a3-f393-e0a9-e50e24dcca9e");
const SimpleBLE::BluetoothUUID NUS_rx("6e400003-b5a3-f393-e0a9-e50e24dcca9e");

std::vector<SimpleBLE::Peripheral> peripherals;

void print_byte_array(SimpleBLE::ByteArray& bytes) {
    for (auto byte : bytes) {
        std::cout << std::hex << (int)byte << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    auto adapter_list = SimpleBLE::Adapter::get_adapters();

    if (adapter_list.size() == 0) {
        std::cout << "No adapter was found." << std::endl;
    }

    // Pick the first detected adapter as the default.
    // TODO: Allow the user to pick an adapter.
    SimpleBLE::Adapter adapter = adapter_list[0];

    adapter.set_callback_on_scan_start([]() { std::cout << "Scan started." << std::endl; });

    adapter.set_callback_on_scan_stop([]() { std::cout << "Scan stopped." << std::endl; });

    adapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) {
        std::cout << "Found device: " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
        peripherals.push_back(peripheral);
    });

    // Scan for 5 seconds and return.
    adapter.scan_for(5000);

    std::cout << "The following devices were found:" << std::endl;
    for (int i = 0; i < peripherals.size(); i++) {
        std::cout << "[" << i << "] " << peripherals[i].identifier() << " [" << peripherals[i].address() << "]"
                  << std::endl;
    }

    int selection = -1;
    std::cout << "Please select a device to connect to: ";
    std::cin >> selection;

    if (selection >= 0 && selection < peripherals.size()) {
        auto peripheral = peripherals[selection];
        std::cout << "Connecting to " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
        peripheral.connect();

        std::cout << "Successfully connected, checking if the NUS service is present..." << std::endl;
        bool service_present = false;
        auto service_list = peripheral.services();
        for (auto service : service_list) {
            if (service.uuid == "6e400001-b5a3-f393-e0a9-e50e24dcca9e") {
                service_present = true;
            }
        }

        if (!service_present) {
            std::cout << "NUS service not found." << std::endl;
            peripheral.disconnect();
        } else {
            // Attempt to read the NUS service's TX characteristic.
            // NOTE: This doesn't work in MacOS yet.
            SimpleBLE::ByteArray rx_data = peripheral.read(NUS_service, NUS_rx);
            std::cout << "RX characteristic contents were: ";
            print_byte_array(rx_data);
        }

        // std::cout << "NUS service is present, writing a message..." << std::endl;
        // auto nus_service = peripheral.service("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
        // auto nus_characteristic = nus_service.characteristic("6e400002-b5a3-f393-e0a9-e50e24dcca9e");
        // nus_characteristic.write("Hello World!");
        // std::cout << "Message written." << std::endl;

        peripheral.disconnect();
    }

    return 0;
}
