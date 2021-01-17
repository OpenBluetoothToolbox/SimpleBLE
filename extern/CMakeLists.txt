cmake_minimum_required(VERSION 3.16.0)

include(FetchContent)

# Detect the operating system and load the necessary dependencies
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message("-- [INFO] Loading Linux dependencies")    

elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows" AND NOT DEFINED CMAKE_SYSTEM_NAME)
    message("-- [INFO] Loading Windows dependencies"
    message(FATAL_ERROR "This operating system is not yet supported.")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    message("-- [INFO] Loading Darwin dependencies"
    message(FATAL_ERROR "This operating system is not yet supported.")
else()
endif()