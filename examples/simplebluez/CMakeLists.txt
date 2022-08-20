cmake_minimum_required(VERSION 3.21)

project(simplebluez_examples)

# Run prelude script to set up environment
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/prelude.cmake)

option(SIMPLEBLUEZ_LOCAL "Use local SimpleBluez" ON)

if (SIMPLEBLUEZ_LOCAL)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../simplebluez ${CMAKE_BINARY_DIR}/simplebluez)
else()
    find_package(simplebluez CONFIG REQUIRED)
endif()

add_subdirectory(list_adapters)
add_subdirectory(list_paired)
add_subdirectory(scan)
add_subdirectory(connect)
add_subdirectory(pair)
add_subdirectory(read)
add_subdirectory(notify)
add_subdirectory(ble_nus)
