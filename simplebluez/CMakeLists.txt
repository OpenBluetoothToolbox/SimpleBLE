cmake_minimum_required(VERSION 3.20)

# Run prelude script to set up environment
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/prelude.cmake)

project(
    simplebluez
    VERSION 0.6.0 # Make sure this matches the version in the changelog.
    DESCRIPTION "A simple C++ wrapper around Bluez with a commercially-friendly licence."
    HOMEPAGE_URL "https://github.com/OpenBluetoothToolbox/SimpleBluez"
    LANGUAGES CXX
)

option(LIBFMT_VENDORIZE "Enable vendorized libfmt" ON)

find_package(fmt REQUIRED)
if(NOT DEFINED DBus1_FOUND)
    find_package(DBus1 REQUIRED)
endif()

if(NOT DEFINED SIMPLEDBUS_SANITIZE AND DEFINED SIMPLEBLUEZ_SANITIZE)
    set(SIMPLEDBUS_SANITIZE ${SIMPLEBLUEZ_SANITIZE})
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../simpledbus ${CMAKE_CURRENT_BINARY_DIR}/simpledbus)

# Load all variables that would eventually need to be exposed to downstream projects
if(NOT SIMPLEDBUS_LOG_LEVEL)
    set(SIMPLEDBUS_LOG_LEVEL "FATAL")
endif()

if(NOT SIMPLEBLUEZ_LOG_LEVEL)
    set(SIMPLEBLUEZ_LOG_LEVEL "FATAL")
endif()

set(SIMPLEBLUEZ_INCLUDE
    ${SIMPLEDBUS_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SIMPLEBLUEZ_SRC
    ${SIMPLEDBUS_SRC}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ProxyOrg.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Logging.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Agent.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Device.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Characteristic.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Exceptions.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ProxyOrgBluez.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Service.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Adapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Bluez.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Descriptor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/Adapter1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/Agent1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/GattDescriptor1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/GattCharacteristic1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/GattService1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/Device1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/Battery1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/AgentManager1.cpp")


# Configure the build targets

add_library(simplebluez ${SIMPLEBLUEZ_SRC})
add_library(simplebluez::simplebluez ALIAS simplebluez)

set_target_properties(
    simplebluez PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    CXX_STANDARD 17
    POSITION_INDEPENDENT_CODE ON
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME simplebluez
    OUTPUT_NAME simplebluez
)

list(APPEND PRIVATE_COMPILE_DEFINITIONS SIMPLEDBUS_LOG_LEVEL=${SIMPLEDBUS_LOG_LEVEL})
list(APPEND PRIVATE_COMPILE_DEFINITIONS SIMPLEBLUEZ_LOG_LEVEL=${SIMPLEBLUEZ_LOG_LEVEL})

target_link_libraries(simplebluez
    PUBLIC ${DBus1_LIBRARIES} pthread
    PRIVATE fmt::fmt-header-only)

target_include_directories(simplebluez
    PRIVATE ${SIMPLEBLUEZ_INCLUDE}
    INTERFACE ${SIMPLEBLUEZ_INCLUDE})

append_sanitize_options("${SIMPLEBLUEZ_SANITIZE}")

apply_build_options(simplebluez
    "${PRIVATE_COMPILE_DEFINITIONS}"
    "${PRIVATE_COMPILE_OPTIONS}"
    "${PRIVATE_LINK_OPTIONS}"
    "${PUBLIC_LINK_OPTIONS}")

# Export important variable for downstream projects

if(NOT STANDALONE)
    set(SIMPLEDBUS_LOG_LEVEL ${SIMPLEDBUS_LOG_LEVEL} PARENT_SCOPE)
    set(SIMPLEBLUEZ_LOG_LEVEL ${SIMPLEBLUEZ_LOG_LEVEL} PARENT_SCOPE)
    set(SIMPLEBLUEZ_INCLUDE ${SIMPLEBLUEZ_INCLUDE} PARENT_SCOPE)
    set(SIMPLEBLUEZ_SRC ${SIMPLEBLUEZ_SRC} PARENT_SCOPE)
endif()