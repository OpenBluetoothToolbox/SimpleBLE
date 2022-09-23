#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "simpleble/SimpleBLE.h"

namespace py = pybind11;

void wrap_characteristic(py::module& m) {
    // TODO: Add __str__ and __repr__ methods
    py::class_<SimpleBLE::Characteristic>(m, "Characteristic")
        .def("uuid", &SimpleBLE::Characteristic::uuid)
        .def("descriptors", &SimpleBLE::Characteristic::descriptors);
}
