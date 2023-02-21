# Configure mcc-lidar package
#
# It defines the following variables
#  MCC_LIDAR_FOUND = MCC_LIDAR_FOUND - TRUE
#  MCC_LIDAR_INCLUDE_DIRS - include directories for mcc-lidar
#  MCC_LIDAR_LIBRARY_DIRS - library directory
#  MCC_LIDAR_LIBRARIES    - the libraries (as targets)
#  MCC_LIDAR_BINARY_DIRS  - the directory for dll and utilites
#  MCC_LIDAR_VERSION      - mcc-lidar library version

message (STATUS "Reading ${CMAKE_CURRENT_LIST_FILE}")
# MCC_LIDAR_VERSION is set by version file
message (STATUS "mcc-lidar configuration, version " ${MCC_LIDAR_VERSION})

# Tell the user project where to find our headers and libraries
get_filename_component (_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
get_filename_component (PROJECT_ROOT_DIR "${_DIR}/../../.." ABSOLUTE)
set (MCC_LIDAR_INCLUDE_DIRS "${PROJECT_ROOT_DIR}/include")
set (MCC_LIDAR_LIBRARY_DIRS "${PROJECT_ROOT_DIR}/lib")
set (MCC_LIDAR_BINARY_DIRS "${PROJECT_ROOT_DIR}/bin")

include(CMakeFindDependencyMacro)
find_dependency(libLAS CONFIG)
include ("${CMAKE_CURRENT_LIST_DIR}$<$<CONFIG:DEBUG>:/debug>/mcc-lidar-depends.cmake")
set (MCC_LIDAR_LIBRARY mcc-lidar)
endif()

# For backwards compatibility
set (MCC_LIDAR_FOUND TRUE)