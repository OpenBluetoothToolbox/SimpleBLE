# Basic CMake configuration
cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(SimpleBLE VERSION 0.1 LANGUAGES CXX)

# Include all necessary CMake modules
include(FetchContent)

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

# Load default parameters passed in through the command line.
if(NOT SIMPLEBLUEZ_GIT_REPOSITORY)
    set(SIMPLEBLUEZ_GIT_REPOSITORY "https://github.com/OpenBluetoothToolbox/SimpleBluez.git")
endif()
if(NOT SIMPLEBLUEZ_GIT_TAG)
    set(SIMPLEBLUEZ_GIT_TAG "master")
endif()

# Perform some overall CMake configurations
#set(CMAKE_CXX_STANDARD 17) // TODO: Specify the standard only for this library

# Configure the external header path
set(SIMPLEBLE_USER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Configure common source files
set(
    SRC_SIMPLEBLE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Adapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Peripheral.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Exceptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AdapterBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PeripheralBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AdapterSafe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PeripheralSafe.cpp
)

set(
    SRC_SIMPLEBLE_C_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src_c/simpleble.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src_c/adapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src_c/peripheral.cpp
)

# Detect the operating system and load the necessary dependencies
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message("-- [INFO] Linux Host Detected")

    if(NOT SIMPLEBLUEZ_LOCAL_PATH)
        FetchContent_Declare(
            simplebluez
            GIT_REPOSITORY ${SIMPLEBLUEZ_GIT_REPOSITORY}
            GIT_TAG ${SIMPLEBLUEZ_GIT_TAG}
        )
        FetchContent_MakeAvailable(simplebluez)
    else()
        message("-- [INFO] Using local SimpleBluez")
        add_subdirectory(${SIMPLEBLUEZ_LOCAL_PATH} ${CMAKE_CURRENT_BINARY_DIR}/simplebluez)
    endif()

    set(
        EXTERNAL_LIBRARIES
        simplebluez-static
        pthread
    )
    set(
        EXTERNAL_INCLUDE_DIRS
        ${SIMPLEBLUEZ_INCLUDES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux
    )
    set(
        SRC_SIMPLEBLE_FILES
        ${SRC_SIMPLEBLE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/AdapterBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/PeripheralBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/Bluez.cpp
    )
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    message("-- [INFO] Windows Host Detected")
    if(CMAKE_GENERATOR_PLATFORM MATCHES "^[Ww][Ii][Nn]32$")
        set(WINDOWS_TARGET_ARCH x86)
    elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Xx]64$")
        set(WINDOWS_TARGET_ARCH x64)
    endif()

    set(
        EXTERNAL_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows
    )
    set(
        SRC_SIMPLEBLE_FILES
        ${SRC_SIMPLEBLE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/AdapterBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/PeripheralBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/Utils.cpp
    )

elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    message("-- [INFO] Darwin Host Detected")
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(
        EXTERNAL_LIBRARIES
        "-framework Foundation"
        "-framework CoreBluetooth"
        ObjC
    )
    set(
        EXTERNAL_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos
    )
    set(
        SRC_SIMPLEBLE_FILES
        ${SRC_SIMPLEBLE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/Utils.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/AdapterBase.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/AdapterBaseMacOS.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/PeripheralBase.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/macos/PeripheralBaseMacOS.mm
    )
else()
    message(FATAL_ERROR "-- [ERROR] UNSUPPORTED SYSTEM: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_SYSTEM_NAME}")
endif()

message("-- [INFO] Building SimpleBLE")

set(
    SIMPLE_BLE_ALL_INCLUDE_DIRS
    ${SIMPLE_BLE_ALL_INCLUDE_DIRS}
    ${SIMPLEBLE_USER_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${EXTERNAL_INCLUDE_DIRS}
)

add_library(simpleble-static STATIC ${SRC_SIMPLEBLE_FILES})
add_library(simpleble SHARED ${SRC_SIMPLEBLE_FILES})
add_library(simpleble-c-static STATIC ${SRC_SIMPLEBLE_C_FILES})
add_library(simpleble-c SHARED ${SRC_SIMPLEBLE_C_FILES})

set_property(TARGET simpleble-static PROPERTY CXX_STANDARD 17)
set_property(TARGET simpleble PROPERTY CXX_STANDARD 17)
set_property(TARGET simpleble-c-static PROPERTY CXX_STANDARD 17)
set_property(TARGET simpleble-c PROPERTY CXX_STANDARD 17)

set_property(TARGET simpleble-static PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET simpleble PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET simpleble-c-static PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET simpleble-c PROPERTY POSITION_INDEPENDENT_CODE ON)

set_property(TARGET simpleble PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)
set_property(TARGET simpleble-c PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_include_directories(simpleble-static PRIVATE ${SIMPLE_BLE_ALL_INCLUDE_DIRS})
target_include_directories(simpleble PRIVATE ${SIMPLE_BLE_ALL_INCLUDE_DIRS})
target_include_directories(simpleble-c-static PRIVATE ${SIMPLE_BLE_ALL_INCLUDE_DIRS})
target_include_directories(simpleble-c PRIVATE ${SIMPLE_BLE_ALL_INCLUDE_DIRS})

target_link_libraries(simpleble-static PUBLIC ${EXTERNAL_LIBRARIES})
target_link_libraries(simpleble PUBLIC ${EXTERNAL_LIBRARIES})
target_link_libraries(simpleble-c-static PUBLIC simpleble-static)
target_link_libraries(simpleble-c PUBLIC simpleble-static)

# Append any compilation flags to each specific target
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")

    set(WINVERSION_CODE 0x0A00) # Selected Windows 10 based on https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt
    
    # Add all the special definitions that need to be added for the program to properly compile on windows.
    
    # /D_WIN32_WINNT -> Specifies the minimum version of Windows that the application is compatible with.
    list(APPEND COMPILE_DEFINITIONS "/D_WIN32_WINNT=${WINVERSION_CODE}")
    # /D_USE_MATH_DEFINES -> Specifies that the math.h header file should be included.
    list(APPEND COMPILE_DEFINITIONS "/D_USE_MATH_DEFINES")

    foreach(DEFINITION ${COMPILE_DEFINITIONS})
        target_compile_definitions(simpleble-static PRIVATE ${DEFINITION})
        target_compile_definitions(simpleble PRIVATE ${DEFINITION})
        target_compile_definitions(simpleble-c-static PRIVATE ${DEFINITION})
        target_compile_definitions(simpleble-c PRIVATE ${DEFINITION})
    endforeach()
    
    # /Gd -> Use __cdecl as the default calling convention. https://docs.microsoft.com/en-us/cpp/cpp/cdecl
    list(APPEND COMPILE_OPTIONS "/Gd")
    # /WX -> Treats all warnings as errors.
    list(APPEND COMPILE_OPTIONS "/WX")
    # /W1 -> Use the lowest level of warnings, as there are some unsafe functions that MSVC doesn't like.
    # TODO: This should be removed once the warnings are fixed.
    list(APPEND COMPILE_OPTIONS "/W1")

    foreach(OPTION ${COMPILE_OPTIONS})
        target_compile_options(simpleble-static PRIVATE ${OPTION})
        target_compile_options(simpleble PRIVATE ${OPTION})
        target_compile_options(simpleble-c-static PRIVATE ${OPTION})
        target_compile_options(simpleble-c PRIVATE ${OPTION})
    endforeach()

elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
endif()

# Export the variables needed by the parent project
if(NOT ${STANDALONE})
    set(SIMPLEBLE_INCLUDES ${SIMPLEBLE_USER_INCLUDE_DIR} PARENT_SCOPE)
endif()
