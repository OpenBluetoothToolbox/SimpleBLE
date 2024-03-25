#include "PeripheralBase.h"

#include "CharacteristicBuilder.h"
#include "DescriptorBuilder.h"
#include "ServiceBuilder.h"

#include <simpleble/Exceptions.h>
#include <algorithm>
#include "CommonUtils.h"
#include "LoggingInternal.h"


using namespace SimpleBLE;
using namespace std::chrono_literals;

PeripheralBase::PeripheralBase() {}

PeripheralBase::~PeripheralBase() {}

void* PeripheralBase::underlying() const { return nullptr; }

std::string PeripheralBase::identifier() { return ""; }

BluetoothAddress PeripheralBase::address() { return BluetoothAddress(); }

BluetoothAddressType PeripheralBase::address_type() { return BluetoothAddressType::UNSPECIFIED; }

int16_t PeripheralBase::rssi() { return 0; }

int16_t PeripheralBase::tx_power() { return 0; }

uint16_t PeripheralBase::mtu() { return 0; }

void PeripheralBase::connect() {}

void PeripheralBase::disconnect() {}

bool PeripheralBase::is_connected() { return false; }

bool PeripheralBase::is_connectable() { return false; }

bool PeripheralBase::is_paired() { return false; }

void PeripheralBase::unpair() {}

std::vector<Service> PeripheralBase::services() { return std::vector<Service>(); }

std::vector<Service> PeripheralBase::advertised_services() { return std::vector<Service>(); }

std::map<uint16_t, ByteArray> PeripheralBase::manufacturer_data() { return std::map<uint16_t, ByteArray>(); }

ByteArray PeripheralBase::read(BluetoothUUID const& service, BluetoothUUID const& characteristic) { return ByteArray(); }

void PeripheralBase::write_request(BluetoothUUID const& service, BluetoothUUID const& characteristic, ByteArray const& data) {}

void PeripheralBase::write_command(BluetoothUUID const& service, BluetoothUUID const& characteristic, ByteArray const& data) {}

void PeripheralBase::notify(BluetoothUUID const& service, BluetoothUUID const& characteristic, std::function<void(ByteArray payload)> callback) {}

void PeripheralBase::indicate(BluetoothUUID const& service, BluetoothUUID const& characteristic, std::function<void(ByteArray payload)> callback) {}

void PeripheralBase::unsubscribe(BluetoothUUID const& service, BluetoothUUID const& characteristic) {}

ByteArray PeripheralBase::read(BluetoothUUID const& service, BluetoothUUID const& characteristic, BluetoothUUID const& descriptor) { return ByteArray(); }

void PeripheralBase::write(BluetoothUUID const& service, BluetoothUUID const& characteristic, BluetoothUUID const& descriptor, ByteArray const& data) {}

void PeripheralBase::set_callback_on_connected(std::function<void()> on_connected) {}

void PeripheralBase::set_callback_on_disconnected(std::function<void()> on_disconnected) {}