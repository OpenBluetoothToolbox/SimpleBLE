cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(simpledbus_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
# gcov discovery of .gcda and .gcno files are impeded by .cpp extension
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_BINARY_DIR}/simpledbus)
find_package(Python3 COMPONENTS Development REQUIRED)
find_package(GTest REQUIRED)

set(
  SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/test_holder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/test_message.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/test_proxy_interfaces.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/test_proxy_children.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/test_path.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/PythonRunner.cpp
)

add_executable(simpledbus_test ${SRC_FILES})
target_link_libraries(simpledbus_test simpledbus::simpledbus ${GTEST_LIBRARIES} ${Python3_LIBRARIES} pthread)
target_include_directories(simpledbus_test PRIVATE ${GTEST_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})

add_custom_command (TARGET simpledbus_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/python/ ${CMAKE_BINARY_DIR}
)

if(SIMPLEDBUS_COVERAGE MATCHES "On")
  find_program(GCOVR_PATH gcovr)
  if(NOT GCOVR_PATH)
    message(FATAL_ERROR "gcovr not found! Aborting...")
  endif()
  add_custom_target(simpledbus_test_coverage
    COMMAND echo "Executing test suite"
    COMMAND GTEST_OUTPUT=xml:results/ GTEST_COLOR=1 ./simpledbus_test
    COMMAND echo "Generating CLI coverage report"
    COMMAND gcovr --root ${CMAKE_CURRENT_LIST_DIR}/.. --exclude ${CMAKE_BINARY_DIR}/_deps --exclude ${CMAKE_CURRENT_LIST_DIR}/src
    COMMAND echo "Generating XML coverage report"
    COMMAND gcovr --xml --root ${CMAKE_CURRENT_LIST_DIR}/.. --exclude ${CMAKE_BINARY_DIR}/_deps  --exclude ${CMAKE_CURRENT_LIST_DIR}/src --output cov.xml
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  add_dependencies(simpledbus_test_coverage simpledbus_test)
endif()
