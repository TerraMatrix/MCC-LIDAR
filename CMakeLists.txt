###############################################################################
#
# Main CMake configuration file for mcc-lidar
#
###############################################################################
# mcc-lidar general settings
project(mcc-lidar)


SET(MCC_LIDAR_VERSION_MAJOR "2")
SET(MCC_LIDAR_VERSION_MINOR "1")
SET(MCC_LIDAR_VERSION_PATCH "0")
set(VERSION "${MCC_LIDAR_VERSION_MAJOR}.${MCC_LIDAR_VERSION_MINOR}.${MCC_LIDAR_VERSION_PATCH}")
SET(MCC_LIDAR_LIB_VERSION "2.1.0")
SET(MCC_LIDAR_LIB_SOVERSION "3")

if(WIN32)
  # Name of C++ library
  set(MCC_LIDAR_LIB_NAME mcc-lidar)

else()
  # Name of C++ library
  set(MCC_LIDAR_LIB_NAME mcc-lidar)
  
endif()

set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

# Choose package components
set(WITH_CUDA TRUE CACHE BOOL "Choose Support for CUDA based dense solvers")

if(WITH_CUDA)
	find_package(CUDAToolkit)
  set(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES "${CUDA_TOOLKIT_ROOT_DIR}/include" CACHE PATH "" FORCE)
  include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
endif()

###############################################################################
# CMake settings
cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_CROSSCOMPILING OFF)

# Allow advanced users to generate Makefiles printing detailed commands
mark_as_advanced(CMAKE_VERBOSE_MAKEFILE)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
set(CMAKE_SKIP_RPATH ON)
endif()

###############################################################################
# General build settings

# Default to debug mode until final 1.6.0 release
if(NOT MSVC_IDE)
  if(NOT CMAKE_BUILD_TYPE)

  set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel" FORCE)
  endif()
    message(STATUS "Setting libLAS build type - ${CMAKE_BUILD_TYPE}")
endif()

# TODO: Still testing the output paths --mloskot
set(MCC_LIDAR_BUILD_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

# Output directory in which to build RUNTIME target files.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MCC_LIDAR_BUILD_OUTPUT_DIRECTORY})

# Output directory in which to build LIBRARY target files
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MCC_LIDAR_BUILD_OUTPUT_DIRECTORY})

# Output directory in which to build ARCHIVE target files.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MCC_LIDAR_BUILD_OUTPUT_DIRECTORY})

###############################################################################
# Platform and compiler specific settings

if(WIN32)
  if (MSVC)

    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
      string(REGEX REPLACE "/W[0-4]" "/W4"
        CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    # Generate dot-user file with user-specific settings for
    # Visual Studio project
    set(MSVC_ENVIRONMENT_PATH "" CACHE STRING
      "Custom PATH for Environment property in Visual Studio for project")
    mark_as_advanced(MSVC_ENVIRONMENT_PATH)
    set(VCPROJ_USER_ENVIRONMENT_PATH_DEBUG "${ENVIRONMENT_PATH}")
    mark_as_advanced(VCPROJ_USER_ENVIRONMENT_PATH_DEBUG)

  endif()

else()

  
endif(WIN32)

if (APPLE)
  set(SO_EXT dylib)
  set(CMAKE_FIND_FRAMEWORK "LAST")
elseif(WIN32)
  set(SO_EXT dll)
else()
  set(SO_EXT so)
endif(APPLE)

###############################################################################
# Search for dependencies

# Boost C++ Libraries support - required
message(STATUS "Searching for Boost 1.42+ - done")

# Default to using static, multithreaded libraries for
# linking under MSVC.  This is because we show users how to
# use boostpro.com installer and install those options when linking
# on windows in the compilation documentation.
if(WIN32)
  if (MSVC)
    set(Boost_USE_STATIC_LIBS   ON)
    set(Boost_USE_MULTITHREADED ON)
  endif(MSVC)
endif(WIN32)

find_package(libLAS)
if(libLAS_FOUND)
	message(STATUS "libLAS FOUND")
  include_directories(${LIBLAS_INCLUDE_DIRS})
else()
	message("libLAS NOT FOUND")
	set(LIBLAS_INCLUDE_DIR "" CACHE PATH "")
	set(LIBLAS_LIBRARY_DEBUG "" CACHE FILEPATH "")
	set(LIBLAS_LIBRARY_RELEASE "" CACHE FILEPATH "")
	mark_as_advanced(CLEAR LIBLAS_INCLUDE_DIR)
  mark_as_advanced(CLEAR LIBLAS_LIBRARY_DEBUG)
  mark_as_advanced(CLEAR LIBLAS_LIBRARY_RELEASE)
  set(LIBLAS_LIBRARY debug    ${LIBLAS_LIBRARY_DEBUG}
                    optimized ${LIBLAS_LIBRARY_RELEASE})
endif()

find_package(Boost 1.42 COMPONENTS program_options system filesystem REQUIRED)

if(Boost_FOUND AND Boost_PROGRAM_OPTIONS_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()





# make these available for the user to set.  
mark_as_advanced(CLEAR Boost_INCLUDE_DIR)
mark_as_advanced(CLEAR Boost_LIBRARY_DIRS)
link_directories(${Boost_LIBRARY_DIRS})


###############################################################################
# Installation settings

if(WIN32)
  set(DEFAULT_LIB_SUBDIR lib)
  set(DEFAULT_DATA_SUBDIR .)
  set(DEFAULT_INCLUDE_SUBDIR include)

  if (MSVC)
    set(DEFAULT_BIN_SUBDIR bin)
  else()
    set(DEFAULT_BIN_SUBDIR .)
  endif()
else()
  # Common locatoins for Unix and Mac OS X
  set(DEFAULT_BIN_SUBDIR bin)
  set(DEFAULT_LIB_SUBDIR lib)
  set(DEFAULT_DATA_SUBDIR share/mcc-lidar)
  set(DEFAULT_INCLUDE_SUBDIR include)
endif()

# Locations are changeable by user to customize layout of libLAS installation
# (default values are platform-specific)
set(MCC_LIDAR_BIN_SUBDIR ${DEFAULT_BIN_SUBDIR} CACHE STRING
  "Subdirectory where executables will be installed")
set(MCC_LIDAR_LIB_SUBDIR ${DEFAULT_LIB_SUBDIR} CACHE STRING
  "Subdirectory where libraries will be installed")
set(MCC_LIDAR_INCLUDE_SUBDIR ${DEFAULT_INCLUDE_SUBDIR} CACHE STRING
  "Subdirectory where header files will be installed")
set(MCC_LIDAR_DATA_SUBDIR ${DEFAULT_DATA_SUBDIR} CACHE STRING
  "Subdirectory where data will be installed")

# Mark *_SUBDIR variables as advanced and dedicated to use by power-users only.
mark_as_advanced(MCC_LIDAR_BIN_SUBDIR
  MCC_LIDAR_LIB_SUBDIR MCC_LIDAR_INCLUDE_SUBDIR MCC_LIDAR_DATA_SUBDIR)

# Full paths for the installation
set(MCC_LIDAR_BIN_DIR ${MCC_LIDAR_BIN_SUBDIR})
set(MCC_LIDAR_LIB_DIR ${MCC_LIDAR_LIB_SUBDIR})
set(MCC_LIDAR_INCLUDE_DIR ${MCC_LIDAR_INCLUDE_SUBDIR})
set(MCC_LIDAR_DATA_DIR ${MCC_LIDAR_DATA_SUBDIR})

###############################################################################
# Installation commands

install(FILES AUTHORS COPYING INSTALL LICENSE.txt README.txt
  DESTINATION ${MCC_LIDAR_DATA_DIR}/doc)

###############################################################################
# Processing of project directories

add_subdirectory(src)

# Add find_package(mcc-lidar) support
add_subdirectory(cmake)

# Version information
SET(CPACK_PACKAGE_VERSION_MAJOR ${MCC_LIDAR_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${MCC_LIDAR_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${MCC_LIDAR_VERSION_MINOR})

SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")



set(CPACK_SOURCE_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${VERSION}")

list(APPEND CPACK_SOURCE_IGNORE_FILES "CMakeCache.txt")
list(APPEND CPACK_SOURCE_IGNORE_FILES "CMakeFiles")

file(GLOB DISTS "mcc-lidar*")
list(APPEND CPACK_SOURCE_IGNORE_FILES ${DISTS})

list(APPEND CPACK_SOURCE_IGNORE_FILES "Makefile")
list(APPEND CPACK_SOURCE_IGNORE_FILES "cmake_install.cmak")
list(APPEND CPACK_SOURCE_IGNORE_FILES "CPackConfig.cmake")
list(APPEND CPACK_SOURCE_IGNORE_FILES "CPackSourceConfig.cmake")
list(APPEND CPACK_SOURCE_IGNORE_FILES "CTestTestfile.cmake")
list(APPEND CPACK_SOURCE_IGNORE_FILES "schemas")
list(APPEND CPACK_SOURCE_IGNORE_FILES "hobu-config.bat")
list(APPEND CPACK_SOURCE_IGNORE_FILES "mpg-config.bat")

include(CPack)

add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

