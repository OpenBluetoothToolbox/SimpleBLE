#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "simpleble/SimpleBLE.h"

namespace py = pybind11;

void wrap_peripheral(py::module& m) {
    // TODO: Add __str__ and __repr__ methods
    py::class_<SimpleBLE::Peripheral>(m, "Peripheral")
        .def("initialized", &SimpleBLE::Peripheral::initialized)
        .def("identifier", &SimpleBLE::Peripheral::identifier)
        .def("address", &SimpleBLE::Peripheral::address)
        .def("rssi", &SimpleBLE::Peripheral::rssi)
        .def("connect", &SimpleBLE::Peripheral::connect, py::call_guard<py::gil_scoped_release>())
        .def("disconnect", &SimpleBLE::Peripheral::disconnect)
        .def("is_connected", &SimpleBLE::Peripheral::is_connected)
        .def("is_connectable", &SimpleBLE::Peripheral::is_connectable)
        .def("is_paired", &SimpleBLE::Peripheral::is_paired)
        .def("unpair", &SimpleBLE::Peripheral::unpair)
        .def("services", &SimpleBLE::Peripheral::services)
        .def("manufacturer_data",
             [](SimpleBLE::Peripheral& p) {
                 std::map<uint16_t, py::bytes> ret;
                 for (auto& kv : p.manufacturer_data()) {
                     ret[kv.first] = py::bytes(kv.second);
                 }
                 return ret;
             })
        .def("read", [](SimpleBLE::Peripheral& p, std::string const& service,
                        std::string const& characteristic) { return py::bytes(p.read(service, characteristic)); })
        .def("write_request", [](SimpleBLE::Peripheral& p, std::string service, std::string characteristic,
                                 py::bytes payload) { p.write_request(service, characteristic, payload); })
        .def("write_command", [](SimpleBLE::Peripheral& p, std::string service, std::string characteristic,
                                 py::bytes payload) { p.write_command(service, characteristic, payload); })
        .def("notify",
             [](SimpleBLE::Peripheral& p, std::string service, std::string characteristic,
                std::function<void(py::bytes payload)> cb) {
                 p.notify(service, characteristic, [cb](SimpleBLE::ByteArray payload) { cb(py::bytes(payload)); });
             })
        .def("indicate",
             [](SimpleBLE::Peripheral& p, std::string service, std::string characteristic,
                std::function<void(py::bytes payload)> cb) {
                 p.indicate(service, characteristic, [cb](SimpleBLE::ByteArray payload) { cb(py::bytes(payload)); });
             })
        .def("unsubscribe", &SimpleBLE::Peripheral::unsubscribe)

        .def("descriptor_read",
             [](SimpleBLE::Peripheral& p, std::string const& service, std::string const& characteristic,
                std::string const& descriptor) { return py::bytes(p.read(service, characteristic, descriptor)); })
        .def("descriptor_write", [](SimpleBLE::Peripheral& p, std::string service, std::string characteristic,
                                    std::string const& descriptor,
                                    py::bytes payload) { p.write(service, characteristic, descriptor, payload); })

        .def("set_callback_on_connected", &SimpleBLE::Peripheral::set_callback_on_connected, py::keep_alive<1, 2>())
        .def("set_callback_on_disconnected", &SimpleBLE::Peripheral::set_callback_on_disconnected,
             py::keep_alive<1, 2>());
}
