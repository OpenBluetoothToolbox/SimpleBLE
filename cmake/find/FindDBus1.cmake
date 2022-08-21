# - Config file for the DBus1 package
# It defines the following variables
#  DBus1_FOUND - Flag for indicating that DBus1 package has been found
#  DBus1_DEFINITIONS  - compile definitions for DBus1 [1]
#  DBus1_INCLUDE_DIRS - include directories for DBus1 [1]
#  DBus1_LIBRARIES    - cmake targets to link against

find_package(PkgConfig REQUIRED)
pkg_search_module(DBus1 REQUIRED dbus-1)

# make the mentioned variables only visible in cmake gui with "advanced" enabled
mark_as_advanced(DBus1_INCLUDE_DIR DBus1_LIBRARY)

set(DBus1_LIBRARIES dbus-1)
set(DBus1_INCLUDE_DIRS "${DBus1_INCLUDE_DIR}" "${DBus1_ARCH_INCLUDE_DIR}")

# setup imported target
add_library(dbus-1 SHARED IMPORTED)
set_property(TARGET dbus-1 APPEND PROPERTY IMPORTED_LOCATION ${DBus1_LIBRARY})
set_property(TARGET dbus-1 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DBus1_INCLUDE_DIRS})
set_property(TARGET dbus-1 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS ${DBus1_DEFINITIONS})