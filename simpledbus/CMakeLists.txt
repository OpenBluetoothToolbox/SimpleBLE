cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/find)

project(
    simpledbus
    VERSION 2.3.0 # Make sure this matches the version in the changelog.
    DESCRIPTION "A simple C++ wrapper around DBus with a commercially-friendly licence."
    HOMEPAGE_URL "https://github.com/OpenBluetoothToolbox/SimpleDBus"
    LANGUAGES CXX
)

option(LIBFMT_VENDORIZE "Enable vendorized libfmt" ON)

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

find_package(fmt REQUIRED)
find_package(DBus1 REQUIRED)

# Load default parameters passed in through the command line.
if(NOT SIMPLEDBUS_LOG_LEVEL)
    set(SIMPLEDBUS_LOG_LEVEL "FATAL")
endif()

# OUTPUTS
message(STATUS "Configuring SimpleDBus")
file(GLOB_RECURSE SRC_SIMPLEDBUS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(simpledbus ${SRC_SIMPLEDBUS_FILES})
add_library(simpledbus::simpledbus ALIAS simpledbus)

set_target_properties(
    simpledbus PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    CXX_STANDARD 17
    POSITION_INDEPENDENT_CODE ON
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME simpledbus
    OUTPUT_NAME simpledbus
)

target_compile_definitions(simpledbus PRIVATE SIMPLEDBUS_LOG_LEVEL=${SIMPLEDBUS_LOG_LEVEL})
target_compile_options(simpledbus PRIVATE -Wfatal-errors -Wpedantic -O3 -Og)

target_link_libraries(simpledbus PUBLIC ${DBus1_LIBRARIES})
target_link_libraries(simpledbus PRIVATE $<BUILD_INTERFACE:fmt::fmt-header-only>)

target_include_directories(simpledbus PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(simpledbus INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_property(TARGET simpledbus APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DBus1_INCLUDE_DIRS})

# Append additional flags for address and thread sanitization
if(SIMPLEDBUS_SANITIZE MATCHES "Address")
    message(STATUS "Appending address sanitization flags")
    set(SANITIZE_ADDRESS_COMPILE_OPTIONS -fsanitize=address -fsanitize-recover=address -fno-omit-frame-pointer -fno-common -g)
    set(SANITIZE_ADDRESS_LINK_OPTIONS -fsanitize=address)

    target_compile_options(simpledbus PUBLIC ${SANITIZE_ADDRESS_COMPILE_OPTIONS})
    target_link_libraries(simpledbus PUBLIC ${SANITIZE_ADDRESS_LINK_OPTIONS})
endif()

if(SIMPLEDBUS_SANITIZE MATCHES "Thread")
    message(STATUS "Appending thread sanitization flags")
    set(SANITIZE_THREAD_COMPILE_OPTIONS -fsanitize=thread -fno-omit-frame-pointer -fno-common -g)
    set(SANITIZE_THREAD_LINK_OPTIONS -fsanitize=thread)

    target_compile_options(simpledbus PUBLIC ${SANITIZE_THREAD_COMPILE_OPTIONS})
    target_link_libraries(simpledbus PUBLIC ${SANITIZE_THREAD_LINK_OPTIONS})
endif()

if(SIMPLEDBUS_COVERAGE MATCHES "On")
    message(STATUS "Appending coverage flags")
    set(COVERAGE_COMPILE_OPTIONS -g -O0 -fprofile-arcs -ftest-coverage)

    target_compile_options(simpledbus PUBLIC ${COVERAGE_COMPILE_OPTIONS})
    target_link_libraries(simpledbus PUBLIC gcov)
endif()

# include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# write_basic_package_version_file(
#     "simpledbusConfigVersion.cmake"
#     COMPATIBILITY SameMajorVersion
# )

# install(
#     TARGETS simpledbus
#     EXPORT simpledbusTargets
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )


# install(
#     FILES cmake/install-config.cmake
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simpledbus
#     RENAME "simpledbusConfig.cmake"
# )

# install(
#     FILES "${PROJECT_BINARY_DIR}/simpledbusConfigVersion.cmake"
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simpledbus
# )

# install(
#     EXPORT simpledbusTargets
#     NAMESPACE simpledbus::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simpledbus
# )

# install(
#     DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/simpledbus/
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simpledbus)


install(
    TARGETS simpledbus
    EXPORT simpledbus-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
    EXPORT simpledbus-config
    NAMESPACE simpledbus::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simpledbus)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/simpledbus/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simpledbus)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/simpledbus.pc.in
            ${CMAKE_CURRENT_BINARY_DIR}/simpledbus.pc @ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/simpledbus.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

