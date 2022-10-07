cmake_minimum_required(VERSION 3.21)

project(simpleble_android_wrapper)

# To simplify logging, we'll bring in libfmt
include(FetchContent)

# Load default parameters passed in through the command line.
set(LIBFMT_GIT_REPOSITORY "https://github.com/fmtlib/fmt.git")
set(LIBFMT_GIT_TAG "b6f4ceaed0a0a24ccf575fab6c56dd50ccf6f1a9") # v8.1.1
FetchContent_Declare(
    fmt
    GIT_REPOSITORY ${LIBFMT_GIT_REPOSITORY}
    GIT_TAG ${LIBFMT_GIT_TAG}
)
FetchContent_MakeAvailable(fmt)

# Print all CMAKE variables
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# Load SimpleBLE
set(SIMPLEBLE_PLAIN ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../../../../../simpleble ${CMAKE_BINARY_DIR}/simpleble)

# Build the wrapper
add_library(simpleble_android SHARED simpleble_android.cpp utils/android_utils.cpp)
set_target_properties(
    simpleble_android PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

target_link_libraries(simpleble_android android log simpleble::simpleble fmt::fmt-header-only)
