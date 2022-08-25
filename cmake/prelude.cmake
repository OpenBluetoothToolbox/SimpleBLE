# Common configuration used by all projects in this repository
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/find)

include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
include(CMakePackageConfigHelpers)

# Detect if the project is being build within a project or standalone.
if(PROJECT_IS_TOP_LEVEL)
    # Configure the build path
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    # Nice hack to automatically ignore the build directory
    file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")
endif()

if(CMAKE_GENERATOR_PLATFORM MATCHES "^[Ww][Ii][Nn]32$")
    set(WINDOWS_TARGET_ARCH x86)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Xx]64$")
    set(WINDOWS_TARGET_ARCH x64)
endif()

macro(apply_build_options
    TARGET
    PRIVATE_COMPILE_DEFINITIONS
    PRIVATE_COMPILE_OPTIONS
    PRIVATE_LINK_OPTIONS
    PUBLIC_LINK_OPTIONS)

    foreach(DEFINITION ${PRIVATE_COMPILE_DEFINITIONS})
        message(STATUS "Applying definition: ${DEFINITION}")
        target_compile_definitions(${TARGET} PRIVATE ${DEFINITION})
    endforeach()

    foreach(OPTION ${PRIVATE_COMPILE_OPTIONS})
        message(STATUS "Applying private compile option: ${OPTION}")
        target_compile_options(${TARGET} PRIVATE ${OPTION})
    endforeach()

    foreach(OPTION ${PRIVATE_LINK_OPTIONS})
        message(STATUS "Applying private link option: ${OPTION}")
        target_link_options(${TARGET} PRIVATE ${OPTION})
    endforeach()

    foreach(OPTION ${PUBLIC_LINK_OPTIONS})
        message(STATUS "Applying public link option: ${OPTION}")
        target_link_options(${TARGET} PUBLIC ${OPTION})
    endforeach()

endmacro()

macro(install_target tgt)
    set(bools)
    set(single EXPORT NAMESPACE VERSION CMAKE_INPUT_FILE CMAKE_OUTPUT_FILE)
    set(multi)

    cmake_parse_arguments(_SBLE "${bools}" "${single}" "${multi}" ${ARGN})

    configure_package_config_file(${_SBLE_CMAKE_INPUT_FILE} ${_SBLE_CMAKE_OUTPUT_FILE}
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${_SBLE_EXPORT})
    write_basic_package_version_file("${_SBLE_EXPORT}-config-version.cmake"
        VERSION ${_SBLE_VERSION}
        COMPATIBILITY SameMajorVersion)

    install(TARGETS ${tgt} fmt-header-only
        EXPORT ${_SBLE_EXPORT}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(EXPORT ${_SBLE_EXPORT}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${_SBLE_EXPORT}
        NAMESPACE ${_SBLE_NAMESPACE}
        FILE "${_SBLE_EXPORT}-targets.cmake"
    )
    install(FILES ${_SBLE_CMAKE_OUTPUT_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}/${_SBLE_EXPORT}-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${_SBLE_EXPORT})
endmacro()

macro(append_sanitize_options MODIFIER)

    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        # Note to self: When dealing with memory issues/leaks on Darwin, the following documentation is useful:
        # - https://clang.llvm.org/docs/AutomaticReferenceCounting.html
        # - https://developer.apple.com/library/archive/documentation/Performance/Conceptual/ManagingMemory/Articles/FindingLeaks.html

        if(${MODIFIER} MATCHES "Address")
            list(APPEND PRIVATE_COMPILE_OPTIONS -fsanitize=address)
            list(APPEND PRIVATE_COMPILE_OPTIONS -fsanitize-recover=address)
            list(APPEND PRIVATE_COMPILE_OPTIONS -fno-omit-frame-pointer)
            list(APPEND PRIVATE_COMPILE_OPTIONS -fno-common)
            list(APPEND PRIVATE_COMPILE_OPTIONS -g)
            list(APPEND PUBLIC_LINK_OPTIONS     -fsanitize=address)

        elseif(${MODIFIER} MATCHES "Thread")
            list(APPEND PRIVATE_COMPILE_OPTIONS -fsanitize=thread)
            list(APPEND PRIVATE_COMPILE_OPTIONS -fno-omit-frame-pointer)
            list(APPEND PRIVATE_COMPILE_OPTIONS -fno-common)
            list(APPEND PRIVATE_COMPILE_OPTIONS -g)
            list(APPEND PUBLIC_LINK_OPTIONS -fsanitize=thread)

        endif()

        elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        # TODO: Figure out how to properly link the MSVC address sanitizer libraries.
        # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address /Zi /MD /DEBUG")
    endif()

endmacro()