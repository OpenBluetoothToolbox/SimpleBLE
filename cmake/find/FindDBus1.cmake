# Attempt to load DBus1 as CONFIG silently
find_package(DBus1 CONFIG QUIET)

# If DBus1 was not found, try to find it as MODULE
if(NOT DBus1_FOUND)

    # - Config file for the DBus1 package
    # It defines the following variables
    #  DBus1_FOUND - Flag for indicating that DBus1 package has been found
    #  DBus1_DEFINITIONS  - compile definitions for DBus1 [1]
    #  DBus1_INCLUDE_DIRS - include directories for DBus1 [1]
    #  DBus1_LIBRARIES    - cmake targets to link against

    find_package(PkgConfig REQUIRED)
    pkg_search_module(PC_DBUS REQUIRED dbus-1)

    set(DBus1_LIBRARIES dbus-1)
    set(DBus1_INCLUDE_DIRS "${DBus1_INCLUDE_DIR}" "${DBus1_ARCH_INCLUDE_DIR}")

    # setup imported target
    add_library(dbus-1 SHARED IMPORTED)
    set_property(TARGET dbus-1 APPEND PROPERTY IMPORTED_LOCATION ${DBus1_LINK_LIBRARIES})
    set_property(TARGET dbus-1 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DBus1_INCLUDE_DIRS})
    set_property(TARGET dbus-1 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS ${DBus1_DEFINITIONS})

    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()

endif()
