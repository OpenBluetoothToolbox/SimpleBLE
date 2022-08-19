cmake_minimum_required(VERSION 3.21)

project(
    simpledbus
    VERSION 2.3.0 # Make sure this matches the version in the changelog.
    DESCRIPTION "A simple C++ wrapper around DBus with a commercially-friendly licence."
    HOMEPAGE_URL "https://github.com/OpenBluetoothToolbox/SimpleDBus"
    LANGUAGES CXX
)

# Run prelude script to set up environment
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/prelude.cmake)

option(LIBFMT_VENDORIZE "Enable vendorized libfmt" ON)

find_package(fmt REQUIRED)
find_package(DBus1 REQUIRED)

# Load all variables that would eventually need to be exposed to downstream projects
if(NOT SIMPLEDBUS_LOG_LEVEL)
    set(SIMPLEDBUS_LOG_LEVEL "FATAL")
endif()

set(SIMPLEDBUS_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SIMPLEDBUS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/advanced/Interface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/advanced/Proxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/base/Connection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/base/Exceptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/base/Holder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/base/Logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/base/Message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/base/Path.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/ObjectManager.cpp)

# Configure the build targets

add_library(simpledbus ${SIMPLEDBUS_SRC})
add_library(simpledbus::simpledbus ALIAS simpledbus)

set_target_properties(
    simpledbus PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    CXX_STANDARD 17
    POSITION_INDEPENDENT_CODE ON
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME simpledbus
    OUTPUT_NAME simpledbus
)

list(APPEND PRIVATE_COMPILE_DEFINITIONS SIMPLEDBUS_LOG_LEVEL=${SIMPLEDBUS_LOG_LEVEL})

target_link_libraries(simpledbus
    PUBLIC ${DBus1_LIBRARIES}
    PRIVATE fmt::fmt-header-only)

target_include_directories(simpledbus
    PRIVATE ${SIMPLEDBUS_INCLUDE}
    INTERFACE ${SIMPLEDBUS_INCLUDE})

append_sanitize_options("${SIMPLEDBUS_SANITIZE}")

apply_build_options(simpledbus
    "${PRIVATE_COMPILE_DEFINITIONS}"
    "${PRIVATE_COMPILE_OPTIONS}"
    "${PRIVATE_LINK_OPTIONS}"
    "${PUBLIC_LINK_OPTIONS}")

if(SIMPLEDBUS_TEST)
    find_package(Python3 COMPONENTS Development REQUIRED)
    find_package(GTest REQUIRED)

    add_executable(simpledbus_test
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/test_holder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/test_message.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/test_proxy_interfaces.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/test_proxy_children.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/test_path.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/src/helpers/PythonRunner.cpp)
    target_link_libraries(simpledbus_test PRIVATE simpledbus::simpledbus ${GTEST_LIBRARIES} ${Python3_LIBRARIES} pthread)
    target_include_directories(simpledbus_test PRIVATE ${GTEST_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})

    add_custom_command (TARGET simpledbus_test POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test/python/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif()