cmake_minimum_required(VERSION 3.20.0)

project(EXAMPLE_WRITE)

message("-- [INFO] Building Example")

add_executable(example_write
    write.cpp
    ../common/utils.cpp
)

target_link_libraries(example_write simpleble::simpleble)
