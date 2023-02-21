// Copyright 2009 Green Code LLC
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

#include <limits>

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "Sequence.h"

// Check the values in a sequence
template <typename T>
void checkSequence(const mcc::Sequence<T> & sequence,
                      const T &           start,
                      const T &           end)
{
  bool isForwardSequence = (start <= end);
  int expectedNValues;
  if (isForwardSequence)
    expectedNValues = end - start + 1;
  else
    expectedNValues = start - end + 1;

  T expectedValue = start;
  int nValues = 0;
  BOOST_FOREACH(T i, sequence) {
    ++nValues;
    BOOST_REQUIRE( nValues <= expectedNValues );
    BOOST_REQUIRE_EQUAL( i, expectedValue );
    if (isForwardSequence)
      ++expectedValue;
    else
      --expectedValue;
  }
  BOOST_REQUIRE_EQUAL( nValues, expectedNValues );
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE( Sequence )

BOOST_AUTO_TEST_CASE( int_forward )
{
  const int start = 7;
  const int end   = 23;
  mcc::Sequence<int> sequence(start, end);
  checkSequence(sequence, start, end);
}

BOOST_AUTO_TEST_CASE( int_reverse )
{
  const int start = -123;
  const int end   = -234;
  mcc::Sequence<int> sequence(start, end);
  checkSequence(sequence, start, end);
}

BOOST_AUTO_TEST_CASE( start_equals_end )
{
  const int start = 99;
  const int end   = start;
  mcc::Sequence<int> sequence(start, end);
  checkSequence(sequence, start, end);
}

BOOST_AUTO_TEST_CASE( ushort_max )
{
  const unsigned short end   = std::numeric_limits<unsigned short>::max();
  const unsigned short start = end - 3;
  mcc::Sequence<unsigned short> sequence(start, end);
  checkSequence(sequence, start, end);
}

BOOST_AUTO_TEST_CASE( all_uchar )
{
  const unsigned char start = std::numeric_limits<unsigned char>::min();
  const unsigned char end   = std::numeric_limits<unsigned char>::max();
  mcc::Sequence<unsigned char> sequence(start, end);
  checkSequence(sequence, start, end);
}

BOOST_AUTO_TEST_CASE( uint_reverse_to_min )
{
  const unsigned int start = 42;
  const unsigned int end   = 0;
  mcc::Sequence<unsigned int> sequence(start, end);
  checkSequence(sequence, start, end);
}

BOOST_AUTO_TEST_SUITE_END()
