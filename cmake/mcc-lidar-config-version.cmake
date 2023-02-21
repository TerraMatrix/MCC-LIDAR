# Version checking for mcc-lidar

set(PACKAGE_VERSION "2.1.0")
set(PACKAGE_VERSION_MAJOR "2")
set(PACKAGE_VERSION_MINOR "1")
set(PACKAGE_VERSION_PATCH "0")
 
if (NOT PACKAGE_FIND_NAME STREQUAL "mcc-lidar")
  # Check package name (in particular, because of the way cmake finds
  # package config files, the capitalization could easily be "wrong").
  # This is necessary to ensure that the automatically generated
  # variables, e.g., <package>_FOUND, are consistently spelled.
  set (REASON "package = mcc-lidar, NOT ${PACKAGE_FIND_NAME}")
  set (PACKAGE_VERSION_UNSUITABLE TRUE)
elseif (NOT (APPLE OR (NOT DEFINED CMAKE_SIZEOF_VOID_P) OR
      CMAKE_SIZEOF_VOID_P EQUAL 8))
  # Reject if there's a 32-bit/64-bit mismatch (not necessary with Apple
  # since a multi-architecture library is built for that platform).
  set (REASON "sizeof(*void) =  8")
  set (PACKAGE_VERSION_UNSUITABLE TRUE)
elseif (MSVC AND NOT MSVC_VERSION STREQUAL "1929")
  # Reject if there's a mismatch in MSVC compiler versions
  set (REASON "_MSC_VER = 1929")
  set (PACKAGE_VERSION_UNSUITABLE TRUE)
elseif (PACKAGE_FIND_VERSION)
  if (PACKAGE_FIND_VERSION VERSION_EQUAL PACKAGE_VERSION)
    set (PACKAGE_VERSION_EXACT TRUE)
  elseif (PACKAGE_FIND_VERSION VERSION_LESS PACKAGE_VERSION
    AND PACKAGE_FIND_VERSION_MAJOR EQUAL PACKAGE_VERSION_MAJOR)
    set (PACKAGE_VERSION_COMPATIBLE TRUE)
  endif ()
endif ()

# If unsuitable, append the reason to the package version so that it's
# visible to the user.
if (PACKAGE_VERSION_UNSUITABLE)
  set (PACKAGE_VERSION "${PACKAGE_VERSION} (${REASON})")
endif ()
