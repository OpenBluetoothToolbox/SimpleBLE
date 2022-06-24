Usage
=====

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
~~~~~~~~~~~

When designing your application using SimpleBLE, concurrency is a key
aspect that needs to be taken into account. This is because internally
the library relies on a thread pool to handle asynchronous operations
and poll the operating system's Bluetooth stack, which can suffer from
contention and potentially cause the program to crash or freeze if these
threads are significantly delayed.

This can have an important effect when using SimpleBLE with UI
applications, such as WxWidgets or Unity.

.. _examples: https://github.com/OpenBluetoothToolbox/SimpleBLE/tree/main/examples

.. _list_adapters: https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/cpp/list_adapters/list_adapters.cpp

.. _scan (cpp): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/cpp/scan/scan.cpp

.. _scan (c): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/c/scan/scan.c

.. _connect (cpp): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/cpp/connect/connect.cpp

.. _connect_safe (cpp): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/cpp/connect_safe/connect_safe.cpp

.. _connect (c): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/c/connect/connect.c

.. _read: https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/cpp/read/read.cpp

.. _write: https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/cpp/write/write.cpp

.. _notify (cpp): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/cpp/notify/notify.cpp

.. _notify (c): https://github.com/OpenBluetoothToolbox/SimpleBLE/blob/main/examples/c/notify/notify.c
