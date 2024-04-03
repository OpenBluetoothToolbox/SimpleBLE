cmake_minimum_required(VERSION 3.22.1)

project("android_examples")

set(BUILD_SHARED_LIBS ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../../../../simpleble ${CMAKE_BINARY_DIR}/simpleble)

add_library(${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        simpleble
        android
        log)