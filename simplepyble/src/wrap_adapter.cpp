#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "simpleble/SimpleBLE.h"

namespace py = pybind11;

void wrap_adapter(py::module& m) {
    // TODO: Add __str__ and __repr__ methods
    py::class_<SimpleBLE::Adapter>(m, "Adapter")
        .def("bluetooth_enabled", &SimpleBLE::Adapter::bluetooth_enabled)
        .def("get_adapters", &SimpleBLE::Adapter::get_adapters)
        .def("initialized", &SimpleBLE::Adapter::initialized)
        .def("identifier", &SimpleBLE::Adapter::identifier)
        .def("address", &SimpleBLE::Adapter::address)
        .def("scan_start", &SimpleBLE::Adapter::scan_start)
        .def("scan_stop", &SimpleBLE::Adapter::scan_stop)
        .def("scan_is_active", &SimpleBLE::Adapter::scan_is_active)
        .def("scan_for", &SimpleBLE::Adapter::scan_for, py::call_guard<py::gil_scoped_release>())
        .def("scan_get_results", &SimpleBLE::Adapter::scan_get_results)
        .def("set_callback_on_scan_start", &SimpleBLE::Adapter::set_callback_on_scan_start, py::keep_alive<1, 2>())
        .def("set_callback_on_scan_stop", &SimpleBLE::Adapter::set_callback_on_scan_stop, py::keep_alive<1, 2>())
        .def("set_callback_on_scan_found", &SimpleBLE::Adapter::set_callback_on_scan_found, py::keep_alive<1, 2>())
        .def("set_callback_on_scan_updated", &SimpleBLE::Adapter::set_callback_on_scan_updated, py::keep_alive<1, 2>())
        .def("get_paired_peripherals", &SimpleBLE::Adapter::get_paired_peripherals);
}
