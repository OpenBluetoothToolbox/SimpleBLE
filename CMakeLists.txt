# Basic CMake configuration
cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(SimpleBLE VERSION 0.1 LANGUAGES CXX)

# Include all necessary CMake modules
include(ExternalProject)

# Detect if the project is being build within a project or standalone.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(STANDALONE true)

    # Configure the build path
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
    set(STANDALONE false)
endif()

# Configure the external header path
set(SIMPLEBLE_USER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Detect the operating system and load the necessary dependencies
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message("-- [INFO] Linux Host Detected")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/linux/SimpleDBus ${CMAKE_BINARY_DIR}/simpledbus)
    set(
        EXTERNAL_LIBRARIES
        bluezdbus-static
        pthread
    )
    set(
        EXTERNAL_INCLUDE_DIRS
        ${SIMPLEDBUS_INCLUDES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux
    )    
    

elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows" AND NOT DEFINED CMAKE_SYSTEM_NAME)
    message("-- [INFO] Windows Host Detected")
    message(FATAL_ERROR "This operating system is not yet supported.")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    message("-- [INFO] Darwin Host Detected")
    message(FATAL_ERROR "This operating system is not yet supported.")
else()
    message(FATAL_ERROR "-- [ERROR] UNSUPPORTED SYSTEM: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_SYSTEM_NAME}")
endif()

message("-- [INFO] Building SimpleBLE")

set(
    SIMPLE_BLE_ALL_INCLUDE_DIRS
    ${SIMPLEBLE_USER_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${EXTERNAL_INCLUDE_DIRS}
)

file(GLOB_RECURSE SRC_SIMPLEBLE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(simpleble-static STATIC ${SRC_SIMPLEBLE_FILES})
add_library(simpleble SHARED ${SRC_SIMPLEBLE_FILES})

set_target_properties(simpleble-static PROPERTIES CXX_STANDARD 17)
set_target_properties(simpleble PROPERTIES CXX_STANDARD 17)

target_include_directories(simpleble-static PRIVATE ${SIMPLE_BLE_ALL_INCLUDE_DIRS})
target_include_directories(simpleble PRIVATE ${SIMPLE_BLE_ALL_INCLUDE_DIRS})

target_link_libraries(simpleble-static PUBLIC ${EXTERNAL_LIBRARIES})
target_link_libraries(simpleble PUBLIC ${EXTERNAL_LIBRARIES})

# Export the variables needed by the parent project
if(NOT ${STANDALONE})
    set(SIMPLEBLE_INCLUDES ${SIMPLEBLE_USER_INCLUDE_DIR} PARENT_SCOPE)
endif()