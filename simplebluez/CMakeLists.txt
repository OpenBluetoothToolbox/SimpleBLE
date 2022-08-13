cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/find")

project(
    simplebluez
    VERSION 0.6.0 # Make sure this matches the version in the changelog.
    DESCRIPTION "A simple C++ wrapper around Bluez with a commercially-friendly licence."
    HOMEPAGE_URL "https://github.com/OpenBluetoothToolbox/SimpleBluez"
    LANGUAGES CXX
)

option(LIBFMT_VENDORIZE "Enable vendorized libfmt" ON)
option(SIMPLEDBUS_VENDORIZE "Enable vendorized SimpleDBus" ON)

# Detect if the project is being build within a project or standalone.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(STANDALONE true)

    # Configure the build path
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    # Nice hack to automatically ignore the build directory
    file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")
else()
    set(STANDALONE false)
endif()

set(SIMPLEDBUS_SANITIZE ${SIMPLEBLUEZ_SANITIZE})
find_package(simpledbus REQUIRED)
find_package(fmt REQUIRED)

if(NOT SIMPLEBLUEZ_LOG_LEVEL)
    set(SIMPLEBLUEZ_LOG_LEVEL "FATAL")
endif()

message(STATUS "Configuring SimpleBluez")
file(GLOB_RECURSE SRC_simplebluez_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(simplebluez ${SRC_simplebluez_FILES})
add_library(simplebluez::simplebluez ALIAS simplebluez)

set_target_properties(
    simplebluez PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    CXX_STANDARD 17
    POSITION_INDEPENDENT_CODE ON
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME simplebluez
    OUTPUT_NAME simplebluez
)

target_compile_definitions(simplebluez PRIVATE SIMPLEBLUEZ_LOG_LEVEL=${SIMPLEBLUEZ_LOG_LEVEL})
target_compile_options(simplebluez PRIVATE -Wfatal-errors -Wpedantic -O3 -Og)

target_link_libraries(simplebluez PRIVATE $<BUILD_INTERFACE:simpledbus::simpledbus> pthread)
target_link_libraries(simplebluez PRIVATE $<BUILD_INTERFACE:fmt::fmt-header-only>)

target_include_directories(simplebluez PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(simplebluez INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Append additional flags for address and thread sanitization
if(SIMPLEBLUEZ_SANITIZE MATCHES "Address")
    message(STATUS "Appending address sanitization flags")
    set(SANITIZE_ADDRESS_COMPILE_OPTIONS -fsanitize=address -fsanitize-recover=address -fno-omit-frame-pointer -fno-common -g)
    set(SANITIZE_ADDRESS_LINK_OPTIONS -fsanitize=address)

    target_compile_options(simplebluez PUBLIC ${SANITIZE_ADDRESS_COMPILE_OPTIONS})
    target_link_libraries(simplebluez PUBLIC ${SANITIZE_ADDRESS_LINK_OPTIONS})
endif()

if(SIMPLEBLUEZ_SANITIZE MATCHES "Thread")
    message(STATUS "Appending thread sanitization flags")
    set(SANITIZE_THREAD_COMPILE_OPTIONS -fsanitize=thread -fno-omit-frame-pointer -fno-common -g)
    set(SANITIZE_THREAD_LINK_OPTIONS -fsanitize=thread)

    target_compile_options(simplebluez PUBLIC ${SANITIZE_THREAD_COMPILE_OPTIONS})
    target_link_libraries(simplebluez PUBLIC ${SANITIZE_THREAD_LINK_OPTIONS})
endif()


if(STANDALONE)
    message(STATUS "Generating install interface for SimpleBluez")

    if (SIMPLEDBUS_VENDORIZE)
        set(SIMPLEDBUS_TARGET simpledbus)
    endif()

    include(GNUInstallDirs)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/simplebluez.pc.in
                ${CMAKE_CURRENT_BINARY_DIR}/simplebluez.pc @ONLY)

    install(
        TARGETS simplebluez ${SIMPLEDBUS_TARGET}
        EXPORT simplebluez-config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(
        EXPORT simplebluez-config
        NAMESPACE simplebluez::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simplebluez)

    install(
        DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/simplebluez/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simplebluez)

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/simplebluez.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

endif()
