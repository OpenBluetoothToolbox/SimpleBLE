#include "AdapterBindings.hpp"

#include "simplersble/simplersble/src/lib.rs.h"

// Adapter Bindings

rust::Vec<SimpleBLE::RustyAdapterWrapper> RustyAdapter_get_adapters() {
    rust::Vec<SimpleBLE::RustyAdapterWrapper> result;

    for (auto& adapter : SimpleBLE::Adapter::get_adapters()) {
        SimpleBLE::RustyAdapterWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyAdapter>(adapter);
        result.push_back(std::move(wrapper));
    }

    return result;
}

bool RustyAdapter_bluetooth_enabled() { return SimpleBLE::Adapter::bluetooth_enabled(); }

void SimpleBLE::RustyAdapter::link(SimpleRsBLE::Adapter& target) const {
    // Time to explain the weird shenanigan we're doing here:
    // The TL;DR is that we're making the Adapter(Rust) and the RustyAdapter(C++)
    // point to each other in a safe way.
    // To achieve this, the Adapter(Rust) owns a RustyAdapter(C++) via a UniquePtr,
    // which ensures that calls will always be made to a valid C++ object.
    // We now give the RustyAdapter(C++) a pointer back to the Adapter(Rust),
    // so that callbacks can be forwarded back to the Rust domain.
    // In order to ensure that the Adapter(Rust) is always valid (given
    // that Rust is keen on moving stuff around) the object is created as a
    // Pin<Box<T>>

    // `_adapter` is a pointer to a pointer, allowing us to manipulate the contents within const functions.
    *_adapter = &target;  // THIS LINE IS SUPER IMPORTANT

    _internal->set_callback_on_scan_start([this]() {
        SimpleRsBLE::Adapter* p_adapter = *this->_adapter;
        if (p_adapter == nullptr) return;

        p_adapter->on_callback_scan_start();
    });

    _internal->set_callback_on_scan_stop([this]() {
        SimpleRsBLE::Adapter* p_adapter = *this->_adapter;
        if (p_adapter == nullptr) return;

        p_adapter->on_callback_scan_stop();
    });

    _internal->set_callback_on_scan_found([this](SimpleBLE::Peripheral peripheral) {
        SimpleRsBLE::Adapter* p_adapter = *this->_adapter;
        if (p_adapter == nullptr) return;

        SimpleBLE::RustyPeripheralWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyPeripheral>(peripheral);
        p_adapter->on_callback_scan_found(wrapper);
    });

    _internal->set_callback_on_scan_updated([this](SimpleBLE::Peripheral peripheral) {
        SimpleRsBLE::Adapter* p_adapter = *this->_adapter;
        if (p_adapter == nullptr) return;

        SimpleBLE::RustyPeripheralWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyPeripheral>(peripheral);
        p_adapter->on_callback_scan_updated(wrapper);
    });
}

void SimpleBLE::RustyAdapter::unlink() const {
    // `_adapter` is a pointer to a pointer.
    *_adapter = nullptr;
}

rust::String SimpleBLE::RustyAdapter::identifier() const { return rust::String(_internal->identifier()); }

rust::String SimpleBLE::RustyAdapter::address() const { return rust::String(_internal->address()); }

void SimpleBLE::RustyAdapter::scan_start() const { _internal->scan_start(); }

void SimpleBLE::RustyAdapter::scan_stop() const { _internal->scan_stop(); }

void SimpleBLE::RustyAdapter::scan_for(int32_t timeout_ms) const { _internal->scan_for(timeout_ms); }

bool SimpleBLE::RustyAdapter::scan_is_active() const { return _internal->scan_is_active(); }

rust::Vec<SimpleBLE::RustyPeripheralWrapper> SimpleBLE::RustyAdapter::scan_get_results() const {
    rust::Vec<SimpleBLE::RustyPeripheralWrapper> result;

    for (auto& peripheral : _internal->scan_get_results()) {
        SimpleBLE::RustyPeripheralWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyPeripheral>(peripheral);
        result.push_back(std::move(wrapper));
    }

    return result;
}

rust::Vec<SimpleBLE::RustyPeripheralWrapper> SimpleBLE::RustyAdapter::get_paired_peripherals() const {
    rust::Vec<SimpleBLE::RustyPeripheralWrapper> result;

    for (auto& peripheral : _internal->get_paired_peripherals()) {
        SimpleBLE::RustyPeripheralWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyPeripheral>(peripheral);
        result.push_back(std::move(wrapper));
    }

    return result;
}

// Peripheral Bindings

void SimpleBLE::RustyPeripheral::link(SimpleRsBLE::Peripheral& target) const {
    // Time to explain the weird shenanigan we're doing here:
    // The TL;DR is that we're making the Peripheral(Rust) and the RustyPeripheral(C++)
    // point to each other in a safe way.
    // To achieve this, the Peripheral(Rust) owns a RustyPeripheral(C++) via a UniquePtr,
    // which ensures that calls will always be made to a valid C++ object.
    // We now give the RustyPeripheral(C++) a pointer back to the Peripheral(Rust),
    // so that callbacks can be forwarded back to the Rust domain.
    // In order to ensure that the Peripheral(Rust) is always valid (given
    // that Rust is keen on moving stuff around) the object is created as a
    // Pin<Box<T>>

    // `_peripheral` is a pointer to a pointer, allowing us to manipulate the contents within const functions.
    *_peripheral = &target;  // THIS LINE IS SUPER IMPORTANT

    _internal->set_callback_on_connected([this]() {
        SimpleRsBLE::Peripheral* p_peripheral = *this->_peripheral;
        if (p_peripheral == nullptr) return;

        p_peripheral->on_callback_connected();
    });

    _internal->set_callback_on_disconnected([this]() {
        SimpleRsBLE::Peripheral* p_peripheral = *this->_peripheral;
        if (p_peripheral == nullptr) return;

        p_peripheral->on_callback_disconnected();
    });
}
void SimpleBLE::RustyPeripheral::unlink() const {
    // `_peripheral` is a pointer to a pointer.
    *_peripheral = nullptr;
}

rust::String SimpleBLE::RustyPeripheral::identifier() const { return rust::String(_internal->identifier()); }

rust::String SimpleBLE::RustyPeripheral::address() const { return rust::String(_internal->address()); }

SimpleBLE::BluetoothAddressType SimpleBLE::RustyPeripheral::address_type() const { return _internal->address_type(); }

int16_t SimpleBLE::RustyPeripheral::rssi() const { return _internal->rssi(); }

int16_t SimpleBLE::RustyPeripheral::tx_power() const { return _internal->tx_power(); }

uint16_t SimpleBLE::RustyPeripheral::mtu() const { return _internal->mtu(); }

void SimpleBLE::RustyPeripheral::connect() const { _internal->connect(); }

void SimpleBLE::RustyPeripheral::disconnect() const { _internal->disconnect(); }

bool SimpleBLE::RustyPeripheral::is_connected() const { return _internal->is_connected(); }

bool SimpleBLE::RustyPeripheral::is_connectable() const { return _internal->is_connectable(); }

bool SimpleBLE::RustyPeripheral::is_paired() const { return _internal->is_paired(); }

void SimpleBLE::RustyPeripheral::unpair() const { _internal->unpair(); }

rust::Vec<SimpleBLE::RustyServiceWrapper> SimpleBLE::RustyPeripheral::services() const {
    rust::Vec<SimpleBLE::RustyServiceWrapper> result;

    for (auto& service : _internal->services()) {
        SimpleBLE::RustyServiceWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyService>(service);
        result.push_back(std::move(wrapper));
    }

    return result;
}

rust::Vec<SimpleBLE::RustyManufacturerDataWrapper> SimpleBLE::RustyPeripheral::manufacturer_data() const {
    rust::Vec<SimpleBLE::RustyManufacturerDataWrapper> result;

    for (auto& manufacturer_data : _internal->manufacturer_data()) {
        SimpleBLE::RustyManufacturerDataWrapper wrapper;
        wrapper.company_id = manufacturer_data.first;

        for (auto& byte : manufacturer_data.second) {
            wrapper.data.push_back(byte);
        }

        result.push_back(std::move(wrapper));
    }

    return result;
}

rust::Vec<uint8_t> SimpleBLE::RustyPeripheral::read(rust::String const& service,
                                                    rust::String const& characteristic) const {
    std::string read_result = _internal->read(std::string(service), std::string(characteristic));

    rust::Vec<uint8_t> result;
    for (auto& byte : read_result) {
        result.push_back(byte);
    }

    return result;
}

void SimpleBLE::RustyPeripheral::write_request(rust::String const& service_rs, rust::String const& characteristic_rs,
                                               rust::Vec<uint8_t> const& data) const {
    std::string service(service_rs);
    std::string characteristic(characteristic_rs);
    std::string data_vec((char*)data.data(), data.size());

    _internal->write_request(service, characteristic, data_vec);
}

void SimpleBLE::RustyPeripheral::write_command(rust::String const& service_rs, rust::String const& characteristic_rs,
                                               rust::Vec<uint8_t> const& data) const {
    std::string service(service_rs);
    std::string characteristic(characteristic_rs);
    std::string data_vec((char*)data.data(), data.size());

    _internal->write_command(service, characteristic, data_vec);
}

void SimpleBLE::RustyPeripheral::notify(rust::String const& service_rs, rust::String const& characteristic_rs) const {
    std::string service(service_rs);
    std::string characteristic(characteristic_rs);

    _internal->notify(service, characteristic, [this, service_rs, characteristic_rs](std::string data) {
        SimpleRsBLE::Peripheral* p_peripheral = *this->_peripheral;
        if (p_peripheral == nullptr) return;

        rust::Vec<uint8_t> data_vec;
        for (auto& byte : data) {
            data_vec.push_back(byte);
        }

        p_peripheral->on_callback_characteristic_updated(service_rs, characteristic_rs, data_vec);
    });
}

void SimpleBLE::RustyPeripheral::indicate(rust::String const& service_rs, rust::String const& characteristic_rs) const {
    std::string service(service_rs);
    std::string characteristic(characteristic_rs);

    _internal->indicate(service, characteristic, [this, service_rs, characteristic_rs](std::string data) {
        SimpleRsBLE::Peripheral* p_peripheral = *this->_peripheral;
        if (p_peripheral == nullptr) return;

        rust::Vec<uint8_t> data_vec;
        for (auto& byte : data) {
            data_vec.push_back(byte);
        }

        p_peripheral->on_callback_characteristic_updated(service_rs, characteristic_rs, data_vec);
    });
}

void SimpleBLE::RustyPeripheral::unsubscribe(rust::String const& service_rs,
                                             rust::String const& characteristic_rs) const {
    std::string service(service_rs);
    std::string characteristic(characteristic_rs);

    _internal->unsubscribe(service, characteristic);
}

rust::Vec<uint8_t> SimpleBLE::RustyPeripheral::read_descriptor(rust::String const& service,
                                                               rust::String const& characteristic,
                                                               rust::String const& descriptor) const {
    std::string read_result = _internal->read(std::string(service), std::string(characteristic),
                                                         std::string(descriptor));

    rust::Vec<uint8_t> result;
    for (auto& byte : read_result) {
        result.push_back(byte);
    }

    return result;
}

void SimpleBLE::RustyPeripheral::write_descriptor(rust::String const& service, rust::String const& characteristic,
                                                  rust::String const& descriptor,
                                                  rust::Vec<uint8_t> const& data) const {
    _internal->write(std::string(service), std::string(characteristic), std::string(descriptor),
                                std::string((char*)data.data(), data.size()));
}

// Service Bindings

rust::Vec<SimpleBLE::RustyCharacteristicWrapper> SimpleBLE::RustyService::characteristics() const {
    rust::Vec<SimpleBLE::RustyCharacteristicWrapper> result;

    for (auto& characteristic : _internal->characteristics()) {
        SimpleBLE::RustyCharacteristicWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyCharacteristic>(characteristic);
        result.push_back(std::move(wrapper));
    }

    return result;
}

// Characteristic Bindings

rust::Vec<SimpleBLE::RustyDescriptorWrapper> SimpleBLE::RustyCharacteristic::descriptors() const {
    rust::Vec<SimpleBLE::RustyDescriptorWrapper> result;

    for (auto& descriptor : _internal->descriptors()) {
        SimpleBLE::RustyDescriptorWrapper wrapper;
        wrapper.internal = std::make_unique<SimpleBLE::RustyDescriptor>(descriptor);
        result.push_back(std::move(wrapper));
    }

    return result;
}