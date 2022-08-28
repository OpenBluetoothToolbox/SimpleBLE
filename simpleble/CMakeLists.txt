cmake_minimum_required(VERSION 3.21)

# TODO: There has to be a better way to do this
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

project(
    simpleble
    VERSION 0.5.0 # Make sure this matches the version in the changelog.
    DESCRIPTION "The ultimate fully-fledged cross-platform library for Bluetooth Low Energy (BLE)."
    HOMEPAGE_URL "https://github.com/OpenBluetoothToolbox/SimpleBLE"
    LANGUAGES CXX
)

# Run prelude script to set up environment
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/prelude.cmake)

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
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
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
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME simpleble-c
    OUTPUT_NAME simpleble-c)

# Configure include directories
target_include_directories(simpleble PRIVATE ${SIMPLEBLE_PRIVATE_INCLUDES})
target_include_directories(simpleble-c PRIVATE ${SIMPLEBLE_PRIVATE_INCLUDES})

target_include_directories(simpleble INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(simpleble-c INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Configure linked libraries
target_link_libraries(simpleble PRIVATE $<BUILD_INTERFACE:fmt::fmt-header-only>)
target_link_libraries(simpleble-c PRIVATE $<BUILD_INTERFACE:fmt::fmt-header-only>)
target_link_libraries(simpleble-c PRIVATE simpleble::simpleble)

append_sanitize_options("${SIMPLEBLE_SANITIZE}")

# Detect the operating system and load the necessary dependencies
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Linux Host Detected")

    find_package(DBus1 REQUIRED)

    if(NOT SIMPLEDBUS_LOG_LEVEL)
        set(SIMPLEDBUS_LOG_LEVEL "FATAL")
    endif()

    if(NOT SIMPLEBLUEZ_LOG_LEVEL)
        set(SIMPLEBLUEZ_LOG_LEVEL "FATAL")
    endif()

    list(APPEND PRIVATE_COMPILE_DEFINITIONS SIMPLEDBUS_LOG_LEVEL=${SIMPLEDBUS_LOG_LEVEL})
    list(APPEND PRIVATE_COMPILE_DEFINITIONS SIMPLEBLUEZ_LOG_LEVEL=${SIMPLEBLUEZ_LOG_LEVEL})

    target_sources(simpleble PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/AdapterBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/PeripheralBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/Bluez.cpp)

    target_sources(simpleble PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/ProxyOrg.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/Logging.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/Agent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/Device.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/Characteristic.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/Exceptions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/ProxyOrgBluez.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/Service.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/Adapter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/Bluez.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/Descriptor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/interfaces/Adapter1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/interfaces/Agent1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/interfaces/GattDescriptor1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/interfaces/GattCharacteristic1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/interfaces/GattService1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/interfaces/Device1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/interfaces/Battery1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/src/interfaces/AgentManager1.cpp
    )

    target_sources(simpleble PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/src/advanced/Interface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/src/advanced/Proxy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/src/base/Connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/src/base/Exceptions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/src/base/Holder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/src/base/Logging.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/src/base/Message.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/src/base/Path.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/src/interfaces/ObjectManager.cpp
    )

    target_link_libraries(simpleble PUBLIC pthread)
    target_link_libraries(simpleble PRIVATE ${DBus1_LIBRARIES})

    target_include_directories(simpleble PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux)
    # target_include_directories(simpleble-c PRIVATE
    #     ${CMAKE_CURRENT_SOURCE_DIR}/../simplebluez/include
    #     ${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus/include)


elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Windows Host Detected")

    set(WINVERSION_CODE 0x0A00) # Selected Windows 10 based on https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # Add all the special definitions for Visual Studio C++

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
    endif()

    target_include_directories(simpleble PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/windows)
    target_sources(simpleble PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/AdapterBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/PeripheralBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/Utils.cpp)

elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Darwin Host Detected")



    list(APPEND PRIVATE_COMPILE_OPTIONS -fobjc-arc)

    target_link_libraries(simpleble PUBLIC "-framework Foundation" "-framework CoreBluetooth" ObjC)
    target_include_directories(simpleble PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/macos)
    target_sources(simpleble PRIVATE
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

include(GNUInstallDirs)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/simpleble.pc.in
            ${CMAKE_CURRENT_BINARY_DIR}/simpleble.pc @ONLY)

install(
    TARGETS simpleble ${SIMPLEDBUS_TARGET}
    EXPORT simpleble-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
    EXPORT simpleble-config
    NAMESPACE simpleble::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simpleble)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/simpleble/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simpleble)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/simpleble.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

if(SIMPLEBLE_TEST)
    message(STATUS "Building Tests")
    find_package(GTest REQUIRED)

    add_executable(simpleble_test
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/test_utils.cpp
    )

    set_target_properties(simpleble_test PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
        CXX_STANDARD 17
        POSITION_INDEPENDENT_CODE ON
        WINDOWS_EXPORT_ALL_SYMBOLS ON)

    target_link_libraries(simpleble_test PRIVATE simpleble::simpleble ${GTEST_LIBRARIES})
    target_include_directories(simpleble_test PRIVATE ${GTEST_INCLUDE_DIRS})
endif()
