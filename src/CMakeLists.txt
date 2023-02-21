
set(MCC_LIDAR_HEADERS_DIR ../include/mcc-lidar)

file(GLOB_RECURSE MCC_LIDAR_HEADERS "${MCC_LIDAR_HEADERS_DIR}/*.hpp" "${MCC_LIDAR_HEADERS_DIR}/*.h")

file (GLOB MCC_LIDAR_SRC_FILES *.cpp)
file (GLOB MAIN_CPP_FILE "main.cpp")
list(REMOVE_ITEM SRC_FILES ${MAIN_CPP_FILE})


source_group("Header Files" FILES ${MCC_LIDAR_HEADERS})
source_group("Source Files" FILES ${MCC_LIDAR_SRC_FILES})

set (TPSDEMO_FILENAMES linalg3d-double.h
                       ludecomposition.h
                       spline.cpp
                       spline.h )
foreach (name ${TPSDEMO_FILENAMES})
  list (APPEND TPSDEMO_SRCS tpsdemo/${name})
endforeach()
source_group("TPS Demo" FILES ${TPSDEMO_SRCS})


include_directories(../include/mcc-lidar)

add_definitions(-DVEC_WITH_DOUBLE_COORDINATES -DMCC_LIDAR_API)

if (MSVC)
  #  Disable warning C4996 about deprecated functions
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)

  #  Enable Win32 structured exceptions along with C++ exceptions
  string(REGEX REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

add_library(${MCC_LIDAR_LIB_NAME} SHARED ${TPSDEMO_SRCS} ${MCC_LIDAR_HEADERS} ${MCC_LIDAR_SRC_FILES})

if(WITH_CUDA)
  target_link_libraries(${MCC_LIDAR_LIB_NAME} PUBLIC
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_PROGRAM_OPTIONS_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${LIBLAS_LIBRARY} CUDA::cudart CUDA::cusparse CUDA::cusolver)
  add_definitions(-DMCC_WITH_CUDA)
else()
  target_link_libraries(${MCC_LIDAR_LIB_NAME} PUBLIC
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_PROGRAM_OPTIONS_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${LIBLAS_LIBRARY})
endif()

set_target_properties(${MCC_LIDAR_LIB_NAME}
  PROPERTIES
  VERSION "${MCC_LIDAR__LIB_VERSION}"
  SOVERSION "${MCC_LIDAR_LIB_SOVERSION}")

install(TARGETS ${MCC_LIDAR_LIB_NAME}
  EXPORT depends
  RUNTIME DESTINATION ${MCC_LIDAR_BIN_DIR}
  LIBRARY DESTINATION ${MCC_LIDAR_LIB_DIR}
  ARCHIVE DESTINATION ${MCC_LIDAR_LIB_DIR})

install(DIRECTORY ${MCC_LIDAR_HEADERS_DIR}
  DESTINATION ${MCC_LIDAR_INCLUDE_DIR})
