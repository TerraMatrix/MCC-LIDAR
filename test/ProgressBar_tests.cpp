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

#include "ProgressBar.h"

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE( LineIndent )

BOOST_AUTO_TEST_CASE( constructor )
{
  std::ostringstream stream;
  stream << "% done: ";
  mcc::ProgressBar progressBar(stream, 1234.56);
  BOOST_REQUIRE_EQUAL( stream.str(), "% done: 0%" );
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( manyUpdates )
{
  std::ostringstream stream;
  mcc::ProgressBar progressBar(stream, 100.0);

  progressBar.update(1.5);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%" );

  // First dot
  progressBar.update(2);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%." );

  // First major tick (10%)
  progressBar.update(11);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10" );

  // No change
  progressBar.update(11);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10" );

  // Big jump
  progressBar.update(29.999);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10....20...." );

  // Tiny change across major tick
  progressBar.update(30);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10....20....30" );

  // Loss of progress (output stream can't be changed)
  progressBar.update(20);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10....20....30" );

  // Made up some ground, but still not caught up
  progressBar.update(26.7);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10....20....30" );

  // Finally made some progress that should be visible
  progressBar.update(32.0001);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10....20....30." );

  // 3/4 of the work is done
  progressBar.update(75);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10....20....30....40....50....60....70.." );

  // Almost all the work is done
  progressBar.update(99.999);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10....20....30....40....50....60....70....80....90...." );

  // All done
  progressBar.update(100);
  BOOST_REQUIRE_EQUAL( stream.str(), "0%....10....20....30....40....50....60....70....80....90....100%" );
}

BOOST_AUTO_TEST_SUITE_END()
