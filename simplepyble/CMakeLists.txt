
# Basic CMake configuration
cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(python_simpleble)

# Include all necessary CMake modules
include(FetchContent)

set(SIMPLEBLE_GIT_REPOSITORY "https://github.com/OpenBluetoothToolbox/SimpleBLE.git")
set(SIMPLEBLE_GIT_TAG "v0.4.0")

FetchContent_Declare(
    simpleble
    GIT_REPOSITORY ${SIMPLEBLE_GIT_REPOSITORY}
    GIT_TAG ${SIMPLEBLE_GIT_TAG}
)
FetchContent_MakeAvailable(simpleble)

find_package(pybind11 REQUIRED)
pybind11_add_module(simplepyble src/main.cpp)

target_include_directories(simplepyble PRIVATE ${SIMPLEBLE_INCLUDES})
target_link_libraries(simplepyble PRIVATE simpleble-static)
