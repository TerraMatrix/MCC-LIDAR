cmake_minimum_required (VERSION 2.6)

project (MCC_LIDAR_UNIT_TESTS CXX)

find_package (Boost 1.39 COMPONENTS filesystem
                                    system
                                    unit_test_framework)

link_directories (${Boost_LIBRARY_DIRS})
include_directories (${Boost_INCLUDE_DIRS})

# Generate a configuration file
configure_file (
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h)
include_directories (${PROJECT_BINARY_DIR})

# Get the root directory for the MCC_LIDAR project
get_filename_component(MCC_LIDAR_ROOT "${PROJECT_SOURCE_DIR}" PATH)
message("MCC_LIDAR_ROOT = ${MCC_LIDAR_ROOT}")

# Search the directory with MCC_LIDAR source files for headers.  That
# directory is a sibling to this test project's source directory.
set (MCC_LIDAR_SRC_DIR  ${MCC_LIDAR_ROOT}/src)
include_directories (${MCC_LIDAR_SRC_DIR})

file (GLOB TEST_SOURCES *.cpp *.h)
list (SORT TEST_SOURCES)
source_group("Test Sources" FILES ${TEST_SOURCES})

set (PROJECT_SRC_NAMES Coordinate.h
                       CoordinateInterval.cpp
                       CoordinateInterval.h
                       DuplicatePoints.cpp
                       DuplicatePoints.h
                       Geometry3D.cpp
                       Geometry3D.h
                       Grid.h
                       GridBase.cpp
                       GridBase.h
                       IPoint.h
                       IPointVector.h
                       IRasterSurface.h
                       IUnclassifiedPoints.h
                       LineIndent.cpp
                       LineIndent.h
                       Point.h
                       PointClassification.h
                       PointVector.cpp
                       PointVector.h
                       PointVectorIterator.h
                       PointVectorIteratorImpl.h
                       ProgressBar.cpp
                       ProgressBar.h
                       RasterSurface.cpp
                       RasterSurface.h
                       Sequence.h
                       StackedPoints.cpp
                       StackedPoints.h
                       UnclassifiedPoints.cpp
                       UnclassifiedPoints.h
                       VectorWrapper.h
                       XYCoordinates.h
                       XyzPoint.h)
foreach (name ${PROJECT_SRC_NAMES})
  list (APPEND PROJECT_SOURCES ${MCC_LIDAR_SRC_DIR}/${name})
endforeach()
source_group("Program Sources" FILES ${PROJECT_SOURCES})

# Have TPSDemo spline code use double-precision for vector coordinates
add_definitions(-DVEC_WITH_DOUBLE_COORDINATES)

set (TPSDEMO_FILENAMES linalg3d-double.h
                       ludecomposition.h
                       spline.cpp
                       spline.h )
foreach (name ${TPSDEMO_FILENAMES})
  list (APPEND TPSDEMO_SOURCES ${MCC_LIDAR_SRC_DIR}/tpsdemo/${name})
endforeach()
source_group("TPSDemo Sources" FILES ${TPSDEMO_SOURCES})

if (MSVC) 
  #  Disable warning C4996 about deprecated functions 
  add_definitions(-D_SCL_SECURE_NO_WARNINGS) 
endif() 

set (PROGRAM_NAME mcc-lidar-unit-tests)
add_executable (${PROGRAM_NAME} ${TEST_SOURCES}
                                ${PROJECT_SOURCES}
                                ${TPSDEMO_SOURCES})
target_link_libraries (${PROGRAM_NAME}
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# Create a command script for Windows to run the tests
if (WIN32)
  configure_file (
    ${PROJECT_SOURCE_DIR}/run-tests.cmd.in
    ${PROJECT_BINARY_DIR}/run-tests.cmd)
endif()
