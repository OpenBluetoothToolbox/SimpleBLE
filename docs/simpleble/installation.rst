Build Instructions
------------------

The project is built using `CMake`_. The basic main flow is as follows:

.. code:: bash

     mkdir build
     cd build
     cmake .. # Replace path to '../examples' to build the examples
     make

The build will generate both a static and a shared library. **TODO:**
Expand this part of the documentation.

A set of helper scripts are provided to simplify the process of building
the library, but are not required.

Windows Requirements
~~~~~~~~~~~~~~~~~~~~

-  `Visual Studio 2019`_
-  Windows SDK 10.0.19041.0 or 10.0.22000.0

**NOTE**: It might be easier to use the bundled helper scripts to build
for Windows.

Linux Requirements
~~~~~~~~~~~~~~~~~~

The Linux implementation of SimpleBLE is built upon `SimpleDBus`_, which
in turn requires `libdbus`_.

- ``libdbus-1-dev`` - On Ubuntu: ``sudo apt-get install libdbus-1-dev``

**NOTE**: See the `Security`_ section for more information on how to
override the default paths for external dependencies.

macOS Requirements
~~~~~~~~~~~~~~~~~~

-  `Xcode Command Line Tools`_

Standalone build from source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

   cd <path-to-simpleble>
   mkdir build && cd build
   cmake ..
   make -j
   sudo make install # Not available yet.

Build as part of another project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you want to add SimpleBLE to your project without any external
dependencies, just clone the repository and link to it on your
``CMakeLists.txt`` file.

::

   add_subdirectory(<path-to-simpleble> ${CMAKE_BINARY_DIR}/simpleble)

Build examples
~~~~~~~~~~~~~~

::

   cd <path-to-simpleble>
   mkdir build && cd build
   cmake ../examples
   make -j

Tests
-----

Testing is currently not available for the library as a whole, yet there
are some build settings that can be used to find issues with the
library.

(Linux, MacOS) Address Sanitizer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to run tests with Address Sanitizer, CMake needs to be called
with the following option: ``-DSIMPLEBLE_SANITIZE=Address``.

When using this feature on MacOS, it's important to use the ``leaks``
application to monitor for memory leaks, as AddressSanitizer does not
support that feature.

(Linux, MacOS) Thread Sanitizer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to run tests with Thread Sanitizer, CMake needs to be called
with the following option: ``-DSIMPLEBLE_SANITIZE=Thread``.

.. code:: cmake

   add_subdirectory(<path-to-simpleble> ${CMAKE_BINARY_DIR}/simpleble)
   include_directories(${SIMPLEBLE_INCLUDES})

.. _CMake: http://cmake.org/
.. _Visual Studio 2019: https://visualstudio.microsoft.com/
.. _SimpleDBus: https://github.com/kdewald/SimpleDBus
.. _libdbus: https://dbus.freedesktop.org/doc/dbus-c/latest/
.. _Security: #security
.. _Xcode Command Line Tools: https://developer.apple.com/xcode/