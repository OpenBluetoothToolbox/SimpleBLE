find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(_dbus_hint QUIET libdbus)
endif()

find_path(DBus1_INCLUDE_DIRS
  NAMES
    dbus/dbus.h
  HINTS
    ${_dbus_hint_INCLUDE_DIRS}
  PATH_SUFFIXES
    include/
    include/dbus-1.0/
    dbus-1.0/
)

if(NOT DBus1_INCLUDE_DIRS)
    set(DBus1_FOUND 0)
    return()
endif()

# setup imported target
add_library(dbus-1 INTERFACE)
set_property(TARGET dbus-1 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DBus1_INCLUDE_DIRS})
set(DBus1_FOUND 1)
