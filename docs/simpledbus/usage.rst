================
SimpleDBus Usage
================

SimpleDBus should work on any Linux environment supporting DBus. To install
the necessary dependencies on Debian-based systems, use the following command: ::

  sudo apt install libdbus-1-dev


Building the Library (Source)
=============================

The included CMake build script can be used to build SimpleDBus.
CMake is freely available for download from https://www.cmake.org/download/. ::

   cd <path-to-simpledbus>
   mkdir build && cd build
   cmake .. -DSIMPLEDBUS_LOG_LEVEL=[VERBOSE|DEBUG|INFO|WARNING|ERROR|FATAL]
   cmake --build . -j7

To build a shared library set the ``BUILD_SHARED_LIBS`` CMake variable to ``TRUE`` ::

  cmake -DBUILD_SHARED_LIBS=TRUE ...


Installing the Library
======================

After building the library you can install it by running :command:`sudo make install`.


Usage with CMake (Installed)
============================

Once SimpleDBus has been installed, it can be consumed from within CMake::

   find_package(simpledbus REQUIRED CONFIG)
   target_link_libraries(<your-target> simpledbus::simpledbus)


Usage with CMake (Local)
=============================

You can add the ``simpledbus`` library directory into your project and include it in
your ``CMakeLists.txt`` file ::

   add_subdirectory(<path-to-simpledbus> ${CMAKE_BINARY_DIR}/simpledbus)
   target_link_libraries(<your-target> simpledbus::simpledbus)


Usage with CMake (Vendorized)
=============================

If you want to use a vendorized copy of SimpleDBus, you can do so by using FetchContent
and specifying the location from where SimpleDBus should be consumed from. ::

   include(FetchContent)
   FetchContent_Declare(
       simpledbus
       GIT_REPOSITORY <simpledbus-git-repository>
       GIT_TAG <simpledbus-git-tag>
       GIT_SHALLOW YES
   )

   # Note that here we manually do what FetchContent_MakeAvailable() would do,
   # except to ensure that the dependency can also get what it needs, we add
   # custom logic between the FetchContent_Populate() and add_subdirectory()
   # calls.
   FetchContent_GetProperties(simpledbus)
   if(NOT simpledbus_POPULATED)
       FetchContent_Populate(simpledbus)
       list(APPEND CMAKE_MODULE_PATH "${simpledbus_SOURCE_DIR}/cmake/find")
       add_subdirectory("${simpledbus_SOURCE_DIR}" "${simpledbus_BINARY_DIR}")
   endif()

You can put this code inside ``Findsimpledbus.cmake`` and add it to your CMake
module path, as depicted in `cmake-init-fetchcontent`_.

Once vendorized using the above approach, you can consume SimpleDBus from
within CMake as you'd normally do ::

   find_package(simpledbus REQUIRED)
   target_link_libraries(<your-target> simpledbus::simpledbus)

One key security feature of SimpleBLE is that it allows the user to specify
the URLs and tags of all internal dependencies, thus allowing compilation
from internal or secure sources without the risk of those getting compromised.

Currently, the following libraries are included as part of SimpleDBus, with
the following CMake options available:

- `fmtlib`_

  - ``LIBFMT_VENDORIZE``: Enable vendorization of fmtlib. *(Default: True)*

  - ``LIBFMT_GIT_REPOSITORY``: The git repository to use for fmtlib.

  - ``LIBFMT_GIT_TAG``: The git tag to use for fmtlib. *(Default: v8.1.1)*

  - ``LIBFMT_LOCAL_PATH``: The local path to use for fmtlib. *(Default: None)*


Generate Documentation
======================

To build documentation for SimpleDBus, you first need to install Sphynx,
using the following commands: ::

   sudo apt install python3-sphinx
   pip3 install sphinx_rtd_theme

Once all dependencies have been installed, HTML documentation can be built
by calling the following commands: ::

   cd <path-to-simpledbus>/docs
   make html


Build Examples
==============

Use the following instructions to build the provided SimpleDBus examples: ::

   cd <path-to-simpledbus>
   mkdir build && cd build
   cmake -DSIMPLEDBUS_LOCAL=ON ../examples
   make -j7


=======
Testing
=======

To build and run unit and integration tests, the following packages are
required: ::

   sudo apt install libgtest-dev libgmock-dev python3-dev
   pip3 install -r <path-to-simpledbus>/test/requirements.txt


Unit Tests
==========

To run the unit tests, run the following command: ::

   cd <path-to-simpledbus>
   mkdir build_test && cd build_test
   cmake -DCMAKE_BUILD_TYPE=Debug ../test
   make -j7
   ./simpledbus_test


Coverage Tests
==============

To run coverage tests, run the following command: ::

   cd <path-to-simpledbus>
   mkdir build_coverage && cd build_coverage
   cmake -DCMAKE_BUILD_TYPE=Debug -DSIMPLEDBUS_COVERAGE=ON ../test
   make -j7 simpledbus_test_coverage


Address Sanitizer Tests
=======================

To run the address sanitizer tests, run the following command: ::

   cd <path-to-simpledbus>
   mkdir build_asan && cd build_asan
   cmake -DCMAKE_BUILD_TYPE=Debug -DSIMPLEDBUS_SANITIZE=Address ../test
   make -j7
   PYTHONMALLOC=malloc ./simpledbus_test

It's important for PYTHONMALLOC to be set to malloc, otherwise the tests will
fail due to Python's memory allocator from triggering false positives.


Thread Sanitizer Tests
=======================

To run the thread sanitizer tests, run the following command: ::

   cd <path-to-simpledbus>
   mkdir build_tsan && cd build_tsan
   cmake -DCMAKE_BUILD_TYPE=Debug -DSIMPLEDBUS_SANITIZE=Thread ../test
   make -j7
   ./simpledbus_test


.. Links

.. _cmake-init-fetchcontent: https://github.com/friendlyanon/cmake-init-fetchcontent
.. _fmtlib: https://github.com/fmtlib/fmt
