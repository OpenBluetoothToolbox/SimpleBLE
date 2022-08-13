cmake_minimum_required(VERSION 3.16)

project(simpledbus_examples)

option(SIMPLEDBUS_LOCAL "Use local SimpleDBus" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if (SIMPLEDBUS_LOCAL)
    message(STATUS "Selecting local SimpleDBus")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_BINARY_DIR}/simpledbus)
else()
    find_package(simpledbus CONFIG REQUIRED)
endif()

add_subdirectory(notification)
