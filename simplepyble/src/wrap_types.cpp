#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "simpleble/Types.h"

namespace py = pybind11;

void wrap_types(py::module& m) {
    py::enum_<SimpleBLE::OperatingSystem>(m, "OperatingSystem")
        .value("WINDOWS", SimpleBLE::OperatingSystem::WINDOWS)
        .value("MACOS", SimpleBLE::OperatingSystem::MACOS)
        .value("LINUX", SimpleBLE::OperatingSystem::LINUX)
        .export_values();

    py::enum_<SimpleBLE::BluetoothAddressType>(m, "BluetoothAddressType")
        .value("PUBLIC", SimpleBLE::BluetoothAddressType::PUBLIC)
        .value("RANDOM", SimpleBLE::BluetoothAddressType::RANDOM)
        .value("UNSPECIFIED", SimpleBLE::BluetoothAddressType::UNSPECIFIED)
        .export_values();

    py::enum_<SimpleBLE::PowerState>(m, "PowerState")
        .value("UNKNOWN", SimpleBLE::PowerState::UNKNOWN)
        .value("POWERED_OFF", SimpleBLE::PowerState::POWERED_OFF)
        .value("POWERED_ON", SimpleBLE::PowerState::POWERED_ON)
        .export_values();
}
