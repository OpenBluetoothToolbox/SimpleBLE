cmake_minimum_required(VERSION 3.21)

project(EXAMPLE_ADVERTISE)

message("-- [INFO] Building Example")
add_executable(example_advertise advertise.cpp)
target_link_libraries(example_advertise simplebluez::simplebluez pthread)
