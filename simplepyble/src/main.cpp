#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "simpleble/Adapter.h"

namespace py = pybind11;

PYBIND11_MODULE(simplepyble, m) {
    m.attr("__version__") = "0.0.5";

    py::class_<SimpleBLE::BluetoothService>(m, "BluetoothService")
        .def_readonly("uuid", &SimpleBLE::BluetoothService::uuid)
        .def_readonly("characteristics", &SimpleBLE::BluetoothService::characteristics);

    // TODO: Add __str__ and __repr__ methods to Peripheral class
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
        .def("read", 
             [](SimpleBLE::Peripheral& p, std::string const& service, std::string const& characteristic) {
                 return py::bytes(p.read(service, characteristic));
             })
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
        .def("set_callback_on_connected", &SimpleBLE::Peripheral::set_callback_on_connected, py::keep_alive<1, 2>())
        .def("set_callback_on_disconnected", &SimpleBLE::Peripheral::set_callback_on_disconnected,
             py::keep_alive<1, 2>());

    // TODO: Add __str__ and __repr__ methods to Adapter class
    py::class_<SimpleBLE::Adapter>(m, "Adapter")
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
