cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(SIMPLEBLUEZ_EXAMPLES)

option(SIMPLEBLUEZ_LOCAL "Use local SimpleBluez" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Nice hack to automatically ignore the build directory
file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if (SIMPLEBLUEZ_LOCAL)
    message(STATUS "Selecting local SimpleBluez")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_BINARY_DIR}/simplebluez)
else()
    find_package(simplebluez CONFIG REQUIRED)
endif()

add_subdirectory(list_adapters)
# add_subdirectory(list_paired)
# add_subdirectory(scan)
# add_subdirectory(connect)
# add_subdirectory(pair)
# add_subdirectory(read)
# add_subdirectory(notify)
# add_subdirectory(ble_nus)
