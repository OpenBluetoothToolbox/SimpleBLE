cmake_minimum_required(VERSION 3.21)

project(EXAMPLE_INCOMING)

message("-- [INFO] Building Example")
add_executable(example_incoming incoming.cpp)
target_link_libraries(example_incoming simplebluez::simplebluez pthread)