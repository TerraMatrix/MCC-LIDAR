cmake_minimum_required(VERSION 3.2)

# CMAKE_TOOLCHAIN_FILE 需要放在 project之前
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED VCPKG_ROOT)
    set(VCPKG_ROOT "$ENV{VCPKG_ROOT}" CACHE PATH "")
  else()
  	set(VCPKG_ROOT "F:/vcpkg/vcpkg" CACHE PATH "")		#set vcpkg root if ENV{VCPKG_ROOT} not defined
endif()
SET(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
  else()
  	set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")		#x64-windows
endif()
set(VCPKG_3RD_PARTY_ROOT "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}")
set(VCPKG_3RD_PARTY_INCLUDE "${VCPKG_3RD_PARTY_ROOT}/include")
if(NOT EXISTS ${VCPKG_3RD_PARTY_ROOT} OR NOT EXISTS ${VCPKG_3RD_PARTY_INCLUDE})
  message(FATAL_ERROR "VCPKG_ROOT: ${VCPKG_ROOT} not available")
endif()
message(STATUS "VCPKG_ROOT: ${VCPKG_ROOT}")

project (MCC_LIDAR CXX)

set (VERSION 2.1)
set (RELEASE "final")

# set default install path
set(MCC_LIDAR_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../install" CACHE PATH "default install path")
SET(CMAKE_INSTALL_PREFIX ${MCC_LIDAR_INSTALL_PATH} CACHE PATH "" FORCE)

# cuda toolkit version must be v11.1, because the third-party library compiled by vcpkg is based on cuda v11.1
set(CUDA_TOOLKIT_ROOT_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.1" CACHE PATH "" FORCE)

include (VersionRelease.cmake)

#  Get the Subversion revision number for the project's source code.  The
#  VersionControl.cmake file tests the variable WINDOWS for the Windows
#  platform, so it needs to be set before including the file.
if (WIN32)
  set (WINDOWS ${WIN32})
  set(CDEGREL $<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>)
endif()

include (VersionControl.cmake)
vcs_get_revision(PROJECT_WC_REVISION)
message(status "Current Subversion revision is ${PROJECT_WC_REVISION}")

set (CONFIG_HEADER "${PROJECT_BINARY_DIR}/config.h")
configure_file ("${PROJECT_SOURCE_DIR}/config.h.in"
                "${CONFIG_HEADER}")
include_directories ("${PROJECT_BINARY_DIR}")


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.76.0 REQUIRED COMPONENTS filesystem
                                    program_options
                                    system)
if (NOT Boost_FOUND AND WIN32 AND NOT DEFINED ENV{BOOST_ROOT})
  #  The FindBoost module doesn't work on Windows with BoostPro's binary
  #  distributions when the subminor version is 0 (e.g., 1.39.0):
  #
  #    http://www.cmake.org/pipermail/cmake/2009-June/030376.html
  #
  #  The BOOST_ROOT environment needs to be set.
  message("Error: Boost was not found."
          "  Set the BOOST_ROOT environment variable to the folder where Boost is installed.")
endif()

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})
find_package (LibLAS)

find_package(CUDAToolkit)
set(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES "${CUDA_TOOLKIT_ROOT_DIR}/include" CACHE PATH "" FORCE)
message("CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES: ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")

link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
include_directories ( ${Boost_INCLUDE_DIRS} ${LIBLAS_INCLUDE_DIRS} )

# FUSION's code for LDA format not integrated yet
#add_subdirectory (fusion)

file (GLOB SRC_FILES *.cpp)
file (GLOB MAIN_CPP_FILE "main.cpp")
# 排除main.cpp，因为是创建动态库
list(REMOVE_ITEM SRC_FILES ${MAIN_CPP_FILE})

file (GLOB HDR_FILES *.h)
list (INSERT HDR_FILES 0 "${CONFIG_HEADER}")

# Have TPSDemo spline code use double-precision for vector coordinates
add_definitions(-DVEC_WITH_DOUBLE_COORDINATES)

set (TPSDEMO_FILENAMES linalg3d-double.h
                       ludecomposition.h
                       spline.cpp
                       spline.h )
foreach (name ${TPSDEMO_FILENAMES})
  list (APPEND TPSDEMO_SRCS tpsdemo/${name})
endforeach()
source_group("TPS Demo" FILES ${TPSDEMO_SRCS})

if (MSVC)
  #  Disable warning C4996 about deprecated functions
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)

  #  Enable Win32 structured exceptions along with C++ exceptions
  string(REGEX REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

set (PROGRAM_NAME mcc-lidar)
set (SDK_PATH mcc-lidar_2.1)

add_library("${PROGRAM_NAME}_lib" SHARED ${SRC_FILES} ${HDR_FILES} ${TPSDEMO_SRCS})
target_link_libraries("${PROGRAM_NAME}_lib" PUBLIC
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_PROGRAM_OPTIONS_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${LIBLAS_LIBRARY} CUDA::cudart CUDA::cusparse CUDA::cusolver)

set_target_properties("${PROGRAM_NAME}_lib" PROPERTIES OUTPUT_NAME ${PROGRAM_NAME})
set_target_properties("${PROGRAM_NAME}_lib" PROPERTIES COMPILE_FLAGS "-DMCC_LIDAR_API")
set_target_properties("${PROGRAM_NAME}_lib"
    PROPERTIES
      CXX_STANDARD 14
      DEBUG_POSTFIX "d")
                       

add_executable("${PROGRAM_NAME}_exe" ${MAIN_CPP_FILE})
target_link_libraries("${PROGRAM_NAME}_exe" PUBLIC "${PROGRAM_NAME}_lib")
set_target_properties("${PROGRAM_NAME}_exe" PROPERTIES OUTPUT_NAME ${PROGRAM_NAME})
set_target_properties("${PROGRAM_NAME}_exe"
    PROPERTIES
      CXX_STANDARD 14
      DEBUG_POSTFIX "d")          

install (TARGETS "${PROGRAM_NAME}_lib" 
    DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin/${CDEGREL}"
    LIBRARY DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/lib/${CDEGREL}"
    ARCHIVE DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/lib/${CDEGREL}"
    RUNTIME DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin/${CDEGREL}"
    )
install(FILES $<TARGET_PDB_FILE:${PROGRAM_NAME}_lib> DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin/${CDEGREL}" OPTIONAL)

foreach (HEAD_FILE ${HDR_FILES})
   install(FILES ${HEAD_FILE} DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/include/mcc-lidar" OPTIONAL)
endforeach()

install (TARGETS "${PROGRAM_NAME}_exe" 
    DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin/${CDEGREL}"
    LIBRARY DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/lib/${CDEGREL}"
    ARCHIVE DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/lib/${CDEGREL}"
    RUNTIME DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin/${CDEGREL}"
    )

if (MSVC)
#  include (InstallRequiredSystemLibraries)
  install (PROGRAMS package/win/set_path.bat DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin")
 
  set (DOC_FILE "Documentation.txt")
  set (PROGRAM_DOC "${PROJECT_BINARY_DIR}/${DOC_FILE}")
  configure_file("${PROJECT_SOURCE_DIR}/package/win/${DOC_FILE}" "${PROGRAM_DOC}")
  install(FILES "${PROGRAM_DOC}" DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/doc")

  install(FILES "$<TARGET_FILE_DIR:${PROGRAM_NAME}_lib>/liblas.dll" DESTINATION 
  "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin/${CDEGREL}" OPTIONAL)

  install(FILES "${CUDA_TOOLKIT_ROOT_DIR}/bin/cudart64_110.dll" DESTINATION 
  "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin/${CDEGREL}" OPTIONAL)

  install(FILES "${CUDA_TOOLKIT_ROOT_DIR}/bin/cusparse64_11.dll" DESTINATION 
  "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin/${CDEGREL}" OPTIONAL)

  install(FILES "${CUDA_TOOLKIT_ROOT_DIR}/bin/cusolver64_11.dll" DESTINATION 
  "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}/bin/${CDEGREL}" OPTIONAL)
endif()

get_filename_component(MCC_LIDAR_ROOT "${PROJECT_SOURCE_DIR}" PATH)
set (LICENSE_FILE "${MCC_LIDAR_ROOT}/LICENSE.txt")
set (NOTICE_FILE  "${MCC_LIDAR_ROOT}/NOTICE.txt")

if (MSVC)
  #  Make sure the license & notice have Windows line endings for Notepad.
  set (LICENSE_FILE_WINDOWS "${PROJECT_BINARY_DIR}/LICENSE.txt")
  configure_file("${LICENSE_FILE}" "${LICENSE_FILE_WINDOWS}")
  install (FILES "${LICENSE_FILE_WINDOWS}" DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}")

  set (NOTICE_FILE_WINDOWS "${PROJECT_BINARY_DIR}/NOTICE.txt")
  configure_file("${NOTICE_FILE}" "${NOTICE_FILE_WINDOWS}")
  install (FILES "${NOTICE_FILE_WINDOWS}" DESTINATION "${MCC_LIDAR_INSTALL_PATH}/${SDK_PATH}")
endif()

set (APP_NAME MCC-LIDAR)
set(CPACK_PACKAGE_DIRECTORY "${MCC_LIDAR_INSTALL_PATH}/bin")
set (CPACK_PACKAGE_VERSION              ${VERSION})
set (CPACK_PACKAGE_VERSION_MAJOR        ${VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MAJOR        ${VERSION_MINOR})
set (CPACK_PACKAGE_FILE_NAME            "${APP_NAME}-${VERSION_RELEASE_ABBR}-setup")
set (CPACK_PACKAGE_INSTALL_DIRECTORY    "${APP_NAME} ${VERSION_RELEASE_ABBR}")
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${APP_NAME} ${VERSION_RELEASE_ABBR}")
set (CPACK_RESOURCE_FILE_LICENSE        "${LICENSE_FILE}")

set (CPACK_OPTIONS_FILE "${PROJECT_BINARY_DIR}/CPackOptions.cmake")
configure_file ("${PROJECT_SOURCE_DIR}/package/CPackOptions.cmake.in"
                "${CPACK_OPTIONS_FILE}"
                @ONLY)
set (CPACK_PROJECT_CONFIG_FILE "${CPACK_OPTIONS_FILE}")

include(CPack)
