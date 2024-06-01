=============
API Reference
=============

The following notes provide an overview of the architecture of some of
the higher-level classes in the library, as to facilitate their
understanding.

Standard API
============

.. doxygenclass:: SimpleBLE::Adapter
   :project: simpleble
   :members:
   :undoc-members:

.. doxygenclass:: SimpleBLE::Peripheral
   :project: simpleble
   :members:
   :undoc-members:

.. doxygenclass:: SimpleBLE::Service
   :project: simpleble
   :members:
   :undoc-members:

.. doxygenclass:: SimpleBLE::Characteristic
   :project: simpleble
   :members:
   :undoc-members:

.. doxygenclass:: SimpleBLE::Descriptor
   :project: simpleble
   :members:
   :undoc-members:

Safe API
========

.. doxygenclass:: SimpleBLE::Safe::Adapter
   :project: simpleble
   :members:
   :undoc-members:

.. doxygenclass:: SimpleBLE::Safe::Peripheral
   :project: simpleble
   :members:
   :undoc-members:

C API
========

.. doxygenfile:: adapter.h
   :project: simpleble

.. doxygenfile:: peripheral.h
   :project: simpleble

Class Diagram
===============

.. mermaid::

   classDiagram
      direction TD
      class Adapter {
         + bool initialized() const
         + void* underlying() const
         + std::string identifier()
         + BluetoothAddress address()
         + void scan_start()
         + void scan_stop()
         + void scan_for(int timeout_ms)
         + bool scan_is_active()
         + std::vector<Peripheral> scan_get_results()
         + void set_callback_on_scan_start(std::function<void()> on_scan_start)
         + void set_callback_on_scan_stop(std::function<void()> on_scan_stop)
         + void set_callback_on_scan_updated(std::function<void(Peripheral)> on_scan_updated)
         + void set_callback_on_scan_found(std::function<void(Peripheral)> on_scan_found)
         + std::vector<Peripheral> get_paired_peripherals()
         + static bool bluetooth_enabled()
         + static std::vector<Adapter> get_adapters()
      }
      class Peripheral {
         + bool initialized() const
         + void* underlying() const
         + std::string identifier()
         + BluetoothAddress address()
         + BluetoothAddressType address_type()
         + int16_t rssi()
         + int16_t tx_power()
         + uint16_t mtu()
         + void connect()
         + void disconnect()
         + bool is_connected()
         + bool is_connectable()
         + bool is_paired()
         + void unpair()
         + std::vector<Service> services()
         + std::map<uint16_t, ByteArray> manufacturer_data()
      }
      class Service {
         + BluetoothUUID uuid()
         + ByteArray data()
         + std::vector<Characteristic> characteristics()
      }
      class Characteristic {
         + BluetoothUUID uuid()
         + std::vector<Descriptor> descriptors()
         + std::vector<std::string> capabilities()
         + bool can_read()
         + bool can_write_request()
         + bool can_write_command()
         + bool can_notify()
         + bool can_indicate()
      }
      class Descriptor {
         + BluetoothUUID uuid()
      }
      Peripheral -- Service : has
      Service -- Characteristic : has
      Characteristic -- Descriptor : has
      Adapter -- Peripheral : read/write
      Peripheral -- Adapter : notifies

