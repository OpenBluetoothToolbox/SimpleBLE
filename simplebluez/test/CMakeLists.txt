cmake_minimum_required(VERSION 3.20)

# Run prelude script to set up environment
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/prelude.cmake)

project(simplebluez_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include SimpleBluez
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_BINARY_DIR}/simplebluez)

find_package(Python3 COMPONENTS Development REQUIRED)

enable_testing()
find_package(GTest REQUIRED)

set(
  SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/PythonRunner.cpp
)

add_executable(simplebluez_test ${SRC_FILES})
target_link_libraries(simplebluez_test simplebluez ${GTEST_LIBRARIES} ${Python3_LIBRARIES})
target_include_directories(simplebluez_test PRIVATE ${GTEST_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS} ${SIMPLEBLUEZ_INCLUDES})

add_custom_command (TARGET simplebluez_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/python/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
