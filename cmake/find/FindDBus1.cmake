find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(_dbus_hint QUIET dbus-1)
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

find_path(DBus1_ARCH_INCLUDE_DIRS
  NAMES
    dbus/dbus-arch-deps.h
  HINTS
    ${_dbus_hint_INCLUDE_DIRS}
  PATH_SUFFIXES
    include/
    include/dbus-1.0/
    dbus-1.0/
    lib/dbus-1.0/include/
    lib64/dbus-1.0/include/
    lib32/dbus-1.0/include/
)

if(NOT DBus1_ARCH_INCLUDE_DIRS)
    set(DBus1_FOUND 0)
    return()
endif()

# setup imported target
add_library(dbus-1 INTERFACE)
add_library(dbus-1::dbus-1 ALIAS dbus-1)
set_property(TARGET dbus-1 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DBus1_INCLUDE_DIRS} ${DBus1_ARCH_INCLUDE_DIRS})
set(DBus1_FOUND 1)
