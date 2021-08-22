cmake_minimum_required(VERSION 3.16.0)

project(EXAMPLE_NORDIC_UART_SERVICE)

message("-- [INFO] Building Example")
add_executable(example_nordic_uart_service nordic_uart_service.cpp)
target_link_libraries(example_nordic_uart_service simpleble-static)
