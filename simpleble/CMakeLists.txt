cmake_minimum_required(VERSION 3.20)

# Run prelude script to set up environment
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/prelude.cmake)

project(
    simpleble
    VERSION 0.5.0 # Make sure this matches the version in the changelog.
    DESCRIPTION "The ultimate fully-fledged cross-platform library for Bluetooth Low Energy (BLE)."
    HOMEPAGE_URL "https://github.com/OpenBluetoothToolbox/SimpleBLE"
    LANGUAGES CXX
)

option(LIBFMT_VENDORIZE "Enable vendorized libfmt" ON)
find_package(fmt REQUIRED)

set(SIMPLEBLE_PRIVATE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/builders
    ${CMAKE_CURRENT_SOURCE_DIR}/src/external)

set(SIMPLEBLE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Adapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Peripheral.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Characteristic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Descriptor.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CharacteristicBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptorBase.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Exceptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AdapterSafe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PeripheralSafe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Logging.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/builders/AdapterBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/builders/PeripheralBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/builders/ServiceBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/builders/CharacteristicBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/builders/DescriptorBuilder.cpp)

set(SIMPLEBLE_C_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src_c/simpleble.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src_c/adapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src_c/peripheral.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src_c/logging.cpp)

# Define targets
add_library(simpleble ${SIMPLEBLE_SRC})
add_library(simpleble-c ${SIMPLEBLE_C_SRC})

add_library(simpleble::simpleble ALIAS simpleble)
add_library(simpleble::simpleble-c ALIAS simpleble-c)

set_target_properties(simpleble PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    CXX_STANDARD 17
    POSITION_INDEPENDENT_CODE ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME simpleble
    OUTPUT_NAME simpleble)

set_target_properties(simpleble-c PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    CXX_STANDARD 17
    POSITION_INDEPENDENT_CODE ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME simpleble-c
    OUTPUT_NAME simpleble-c)

# Configure include directories
target_include_directories(simpleble PRIVATE ${SIMPLEBLE_PRIVATE_INCLUDES})
target_include_directories(simpleble-c PRIVATE ${SIMPLEBLE_PRIVATE_INCLUDES})
target_include_directories(simpleble INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(simpleble-c INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Configure linked libraries
target_link_libraries(simpleble PRIVATE fmt::fmt-header-only)
target_link_libraries(simpleble-c PRIVATE fmt::fmt-header-only)
target_link_libraries(simpleble-c PRIVATE simpleble::simpleble)

append_sanitize_options("${SIMPLEBLE_SANITIZE}")

# Detect the operating system and load the necessary dependencies
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Linux Host Detected")

    if(NOT DEFINED DBus1_FOUND)
        find_package(DBus1 REQUIRED)
    endif()

    if(NOT DEFINED SIMPLEBLUEZ_SANITIZE AND DEFINED SIMPLEBLE_SANITIZE)
        set(SIMPLEBLUEZ_SANITIZE ${SIMPLEBLE_SANITIZE})
    endif()

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez ${CMAKE_CURRENT_BINARY_DIR}/simplebluez)

    target_link_libraries(simpleble PUBLIC ${DBus1_LIBRARIES} pthread)
    target_include_directories(simpleble
        PRIVATE
        ${SIMPLEBLUEZ_INCLUDE}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux)
    target_include_directories(simpleble-c PRIVATE ${SIMPLEBLUEZ_INCLUDE})
    target_sources(simpleble
        PUBLIC
        ${SIMPLEDBUS_SRC}
        ${SIMPLEBLUEZ_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/AdapterBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/PeripheralBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/Bluez.cpp)

elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Windows Host Detected")

    set(WINVERSION_CODE 0x0A00) # Selected Windows 10 based on https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt

    # Add all the special definitions that need to be added for the program to properly compile on windows.

    # /D_WIN32_WINNT -> Specifies the minimum version of Windows that the application is compatible with.
    list(APPEND PRIVATE_COMPILE_DEFINITIONS "/D_WIN32_WINNT=${WINVERSION_CODE}")
    # /D_USE_MATH_DEFINES -> Specifies that the math.h header file should be included.
    list(APPEND PRIVATE_COMPILE_DEFINITIONS "/D_USE_MATH_DEFINES")

    # /Gd -> Use __cdecl as the default calling convention. https://docs.microsoft.com/en-us/cpp/cpp/cdecl
    list(APPEND PRIVATE_COMPILE_OPTIONS "/Gd")
    # /WX -> Treats all warnings as errors.
    list(APPEND PRIVATE_COMPILE_OPTIONS "/WX")
    # /W1 -> Use the lowest level of warnings, as there are some unsafe functions that MSVC doesn't like.
    # TODO: This should be removed once the warnings are fixed.
    list(APPEND PRIVATE_COMPILE_OPTIONS "/W1")

    target_include_directories(simpleble PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/windows)
    target_sources(simpleble
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/AdapterBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/PeripheralBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/Utils.cpp)

elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Darwin Host Detected")
    set(CMAKE_CXX_EXTENSIONS OFF)

    list(APPEND PRIVATE_COMPILE_OPTIONS -fobjc-arc)

    target_link_libraries(simpleble PUBLIC "-framework Foundation" "-framework CoreBluetooth" ObjC)
    target_include_directories(simpleble PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/macos)
    target_sources(simpleble
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/Utils.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/AdapterBase.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/AdapterBaseMacOS.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/PeripheralBase.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/PeripheralBaseMacOS.mm)

else()
    message(FATAL_ERROR "-- [ERROR] UNSUPPORTED SYSTEM: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_SYSTEM_NAME}")
endif()

apply_build_options(simpleble
    "${PRIVATE_COMPILE_DEFINITIONS}"
    "${PRIVATE_COMPILE_OPTIONS}"
    "${PRIVATE_LINK_OPTIONS}"
    "${PUBLIC_LINK_OPTIONS}")

apply_build_options(simpleble-c
    "${PRIVATE_COMPILE_DEFINITIONS}"
    "${PRIVATE_COMPILE_OPTIONS}"
    "${PRIVATE_LINK_OPTIONS}"
    "${PUBLIC_LINK_OPTIONS}")

# Provide installation interface

#     include(GNUInstallDirs)

#     configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/simplebluez.pc.in
#                 ${CMAKE_CURRENT_BINARY_DIR}/simplebluez.pc @ONLY)

#     install(
#         TARGETS simplebluez ${SIMPLEDBUS_TARGET}
#         EXPORT simplebluez-config
#         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#     install(
#         EXPORT simplebluez-config
#         NAMESPACE simplebluez::
#         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simplebluez)

#     install(
#         DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/simplebluez/
#         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simplebluez)

#     install(
#         FILES ${CMAKE_CURRENT_BINARY_DIR}/simplebluez.pc
#         DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)