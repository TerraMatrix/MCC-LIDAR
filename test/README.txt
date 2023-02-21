Building the Unit Tests
-----------------------

1)  Read the file "../README.txt" about prerequisites.

2)  Generate the Project Files

    Run CMake to configure and generate the project files for the unit tests.
    Configure the project to do an out-of-source build.  We recommend naming
    the binary directory after the platform and placing it in the MCC-LiDAR
    "build/test/" directory, for example:

      Source directory:  {MCC LiDAR root}/test

      Binary directory:  {MCC LiDAR root}/build/test/{platform}
               example:  {MCC LiDAR root}/build/test/os-x
               example:  {MCC LiDAR root}/build/test/windows

3)  If the Boost libraries are not installed in a standard location, then CMake
    may not find them when configuring the project.  If so, you'll have to
    specify their location with the "Boost_INCLUDE_DIR" property in the
    Advanced View of the CMake GUI.

4)  Build the Tests

    On OS X, open the Xcode project file, for example,

      {MCC LiDAR root}/build/os-x/MCC_LIDAR_UNIT_TESTS.xcodeproj

    and click Build icon in the toolbar.

    On Windows, open the Visual Studio solution in the build directory, and
    build the solution.


Running the Tests
-----------------

Run the test executable from the command prompt:

  OS X:     .../build/test/os-x/Debug/mcc-lidar-unit-tests

  Windows:  cd ...\build\test\windows
            .\run-tests.cmd Debug

Consult the user guide for the Boost.Test library about runtime configuration
using the "--log_level" and "--run_test" options.
