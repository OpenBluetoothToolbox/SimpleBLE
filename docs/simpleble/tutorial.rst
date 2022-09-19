========
Tutorial
========

More coming soon, I swear. :P

Getting Started
===============

Your first step towards using SimpleBLE is to download and install the library
following the instructions in the `usage <usage.html>`_ page. Once you have
installed the library, you need to understand a few basic concepts about how
SimpleBLE is organized.

In SimpleBLE, everything revolves around two main object types: An ``Adapter``
representing a physical Bluetooth adapter, and a ``Peripheral`` representing the
Bluetooth device you are communicating with.

Your entry point to any interaction is the ``Adapter::get_adapters`` function,
which returns a list of all available Bluetooth adapters.

Examples
========

To learn how to use SimpleBLE, please refer to the `examples`_ provided
in the repository. Those examples named with a ``_c`` suffix are using
the C-wrapped version of the library and those examples with a ``_safe``
suffix use the _noexcept_ version of the library.

The following list briefly describes each example provided:

* `list_adapters`_: List all available adapters.
* `scan (cpp)`_ & `scan (c)`_: Scan for nearby BLE devices.
* `connect (cpp)`_ & `connect_safe (cpp)`_ & `connect (c)`_: Connect to a BLE device and list its services and characteristics.
* `read`_: Read a characteristic's value.
* `write`_: Write a characteristic's value.
* `notify (cpp)`_ & `notify (c)`_: Enable notifications on a characteristic.

.. Links

.. _examples: https://github.com/OpenBluetoothToolbox/SimpleBLE/tree/main/examples/simpleble

.. _list_adapters: https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/cpp/list_adapters/list_adapters.cpp

.. _scan (cpp): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/cpp/scan/scan.cpp

.. _scan (c): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/c/scan/scan.c

.. _connect (cpp): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/cpp/connect/connect.cpp

.. _connect_safe (cpp): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/cpp/connect_safe/connect_safe.cpp

.. _connect (c): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/c/connect/connect.c

.. _read: https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/cpp/read/read.cpp

.. _write: https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/cpp/write/write.cpp

.. _notify (cpp): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/cpp/notify/notify.cpp

.. _notify (c): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/simpleble/c/notify/notify.c
