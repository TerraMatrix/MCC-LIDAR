#############################################################################
# Config file generation and installation
#############################################################################

# Set where cmake will install mcc-lidar-config.cmake.  It's installed in
# ${CMAKE_INSTALL_PREFIX}/${INSTALL_CMAKE_DIR} and ${PROJECT_ROOT_DIR}
# is the relative path to the root from there.
if (NOT WIN32)
  set(INSTALL_CMAKE_DIR "share/cmake/${PROJECT_NAME}")
  set (PROJECT_ROOT_DIR "../../..")
else ()
  set(INSTALL_CMAKE_DIR "cmake")
  set (PROJECT_ROOT_DIR "..")
endif ()

# Now create the mcc-lidar-config files using the .in templates
configure_file (mcc-lidar-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/mcc-lidar-config.cmake @ONLY)
configure_file (mcc-lidar-config-version.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/mcc-lidar-config-version.cmake @ONLY)

# Install the mcc-lidar-config files so that other modules can find this
# project using 'find_package(mcc-lidar)'
install (FILES
  "${CMAKE_CURRENT_BINARY_DIR}/mcc-lidar-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/mcc-lidar-config-version.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}")

# Make information about mcc-lidar available.  The depends target is
# defined in ../src/CMakeLists.txt
install (EXPORT depends
  FILE "mcc-lidar-depends.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}")
