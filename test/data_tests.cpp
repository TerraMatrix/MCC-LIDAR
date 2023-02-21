#define BOOST_TEST_DYN_LINK

#include <string>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/test/unit_test.hpp>

#include "data.h"

BOOST_AUTO_TEST_SUITE( dataFunctions )

BOOST_AUTO_TEST_CASE( helloWorld )
{
  namespace filesys = boost::filesystem;

  filesys::path path( testDataDir() / "HelloWorld.txt" );

  BOOST_REQUIRE( filesys::exists(path) );
  BOOST_REQUIRE( filesys::is_regular_file(path) );

  filesys::ifstream file;
  file.open(path);
  BOOST_REQUIRE( file.good() );

  std::string line;
  BOOST_REQUIRE( std::getline(file, line) );

  BOOST_CHECK_EQUAL( line, "Hello World!");
}

BOOST_AUTO_TEST_SUITE_END()
