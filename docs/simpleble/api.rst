=============
API Reference
=============

The following notes provide an overview of the architecture of some of
the higher-level classes in the library, as to facilitate their
understanding.

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

