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

Concurrency
-----------

When designing your application using SimpleBLE, concurrency is a key
aspect that needs to be taken into account. This is because internally
the library relies on a thread pool to handle asynchronous operations
and poll the operating system's Bluetooth stack, which can suffer from
contention and potentially cause the program to crash or freeze if these
threads are significantly delayed.

This can have an important effect when using SimpleBLE with UI
applications, such as WxWidgets or Unity.

Layers and their responsibilities
---------------------------------

-  External layer

   -  ``SimpleBLE::Adapter`` and ``SimpleBLE::Peripheral`` classes.
   -  These objects hold a shared pointer to ``SimpleBLE::AdapterBase``
      and ``SimpleBLE::PeripheralBase`` respectively.

-  C-style wrapper layer

   -  This layer is a C-style wrapper around the safe C++, designed to
      allow integration of SimpleBLE into other languages that have
      support for C bindings.

-  Safe layer

   -  ``SimpleBLE::AdapterSafe`` and ``SimpleBLE::PeripheralSafe`` classes.
   -  These objects wrap all ``SimpleBLE::Adapter`` and
      ``SimpleBLE::Peripheral`` objects and provide an interface that
      does not throw exceptions. Instead, it will return an
      ``std::optional<T>`` object if the function returns a value, or a
      boolean indicating whether the function succeeded if the original
      function did not return a value. The usage is functionally
      equivalent to their respective counterparts in the external layer.

-  API layer (OS-dependent)

   -  ``SimpleBLE::AdapterBase`` and ``SimpleBLE::PeripheralBase``
      classes.
   -  These classes specify the API of the library on top of which the
      external layer is actually wrapping.
   -  Each OS target has to implement the full public API specified in
      the external layer, using private methods and properties for the
      specific requirements of each environment.
   -  Two convenience classes, ``SimpleBLE::AdapterBuilder`` and
      ``SimpleBLE::PeripheralBuilder`` are provided for the case of
      allowing access to private methods during the build process.

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
