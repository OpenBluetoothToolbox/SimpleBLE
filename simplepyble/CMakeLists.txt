cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

project(simplepyble)

# Run prelude script to set up environment
include(${CMAKE_CURRENT_LIST_DIR}/../cmake/prelude.cmake)

find_package(pybind11 REQUIRED)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../simpleble ${CMAKE_BINARY_DIR}/simpleble)

pybind11_add_module(simplepyble src/main.cpp)
target_link_libraries(simplepyble PRIVATE simpleble::simpleble)

set_target_properties(
    simplepyble PROPERTIES
    CXX_STANDARD 17
)