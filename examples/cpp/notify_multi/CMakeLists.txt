cmake_minimum_required(VERSION 3.16.0)

project(EXAMPLE_NOTIFY_MULTI)

message("-- [INFO] Building Example")

add_executable(example_notify_multi
    notify_multi.cpp
    ../common/utils.cpp
)

target_link_libraries(example_notify_multi simpleble-static)
