MCC LiDAR Program
=================

The directory structure of this project:

  project/ ............... The project's root directory
    src/   ............... Source files for the program 
    test/  ............... Unit tests
    build/ ............... Where the program and tests are built


Prerequisites for Building the Program & Its Tests
--------------------------------------------------

  *  CMake - cross-platform build system

     It produces project files for the developer tools native to your
     platform.  It's available in binary form for various platforms from
	 http://www.cmake.org/.  Download and install the executable for your
	 platform.

  *  C++ compiler

     On OS X, you can use Apple's Xcode IDE.
     On Windows, you can use Visual C++ Express Edition.

  *  Boost C++ libraries, version 1.51

     They are available at http://www.boost.org/.  They come pre-built and
     installed on popular Unix and Linux systems.  You'll have to download
     them if you're on OS X or Windows.  The Boost "Getting Started Guide"
     describes how to install the libraries.
     
     Note: Some of the Boost libraries used by the program and its tests
     have binaries (i.e., they are not header-only libraries):

       Library binaries       Used by (P=program, T=tests)
       ----------------       -------
       Boost.Filesystem       P   T
       Boost.ProgramOptions   P    
       Boost.System           P   T
       Boost.Test                 T

  *  libLAS library, version 1.7.0

     This library is available from http://www.liblas.org/.  On Windows, 
	 install the libLAS package in OSGeo4W, and then set the LIBLAS_ROOT 
	 environment accordingly.


Building the Program
--------------------

See the file "src/README.txt for details.


Building the Unit Tests
-----------------------

See the file "test/README.txt for details.
