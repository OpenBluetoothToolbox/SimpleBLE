#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "simpleble/SimpleBLE.h"

namespace py = pybind11;

void wrap_descriptor(py::module& m) {
    // TODO: Add __str__ and __repr__ methods
    py::class_<SimpleBLE::Descriptor>(m, "Descriptor").def("uuid", &SimpleBLE::Descriptor::uuid);
}
