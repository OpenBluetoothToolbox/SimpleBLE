cmake_minimum_required(VERSION 3.20.0)

project(EXAMPLE_SCAN)

message("-- [INFO] Building Example")

add_executable(example_scan
    scan.cpp
    ../common/utils.cpp
)

target_link_libraries(example_scan simpleble::simpleble)
