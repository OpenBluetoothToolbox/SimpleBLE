#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "simpleble/SimpleBLE.h"

namespace py = pybind11;

void wrap_service(py::module& m) {
    // TODO: Add __str__ and __repr__ methods
    py::class_<SimpleBLE::Service>(m, "Service")
        .def("uuid", &SimpleBLE::Service::uuid)
        .def("characteristics", &SimpleBLE::Service::characteristics);
}
