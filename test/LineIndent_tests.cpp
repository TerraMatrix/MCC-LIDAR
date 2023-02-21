// Copyright 2010 Green Code LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#define BOOST_TEST_DYN_LINK

#include <sstream>

#include <boost/test/unit_test.hpp>

#include "LineIndent.h"

namespace {
  struct Fixture
  {
    std::ostringstream stream;
    std::string fooBar_indent;
    std::string fooBar_text;

    Fixture()
      :  fooBar_indent("--> "),
         fooBar_text("Foo Bar")
    {
    }

    void fooBar()
    {
      mcc::LineIndent indent(fooBar_indent);
      stream << indent << fooBar_text << std::endl;
    }
  };
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE( LineIndent , Fixture )

BOOST_AUTO_TEST_CASE( level1 )
{
  mcc::LineIndent indent(". ");
  stream.str("");
  stream << indent << "Hello world!" << std::endl;
  BOOST_REQUIRE_EQUAL( stream.str(), ". Hello world!\n" );
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( level2 )
{
  mcc::LineIndent indent("+ ");
  stream.str("");
  stream << indent << "Hello World" << std::endl;
  fooBar();
  stream << indent << "Goodbye World" << std::endl;
  std::ostringstream expectedStr;
  expectedStr << "+ Hello World\n"
              << "+ " << fooBar_indent << fooBar_text << "\n"
              << "+ Goodbye World\n";
  BOOST_REQUIRE_EQUAL( stream.str(), expectedStr.str() );
}

BOOST_AUTO_TEST_SUITE_END()
