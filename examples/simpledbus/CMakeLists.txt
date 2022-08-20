cmake_minimum_required(VERSION 3.21)

project(simpledbus_examples)

# Run prelude script to set up environment
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/prelude.cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SIMPLEDBUS_LOCAL "Use local SimpleDBus" ON)

if (SIMPLEDBUS_LOCAL)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../simpledbus ${CMAKE_BINARY_DIR}/simpledbus)
else()
    find_package(simpledbus CONFIG REQUIRED)
endif()

add_subdirectory(notification)
