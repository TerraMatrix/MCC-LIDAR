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

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "PointVector.h"
#include "TestUtil.h"

BOOST_AUTO_TEST_SUITE( PointVector )

BOOST_AUTO_TEST_CASE( ctor )
{
  const mcc::IPointVector::size_type nPoints = 123;
  mcc::PointVector points(nPoints);

  BOOST_CHECK_EQUAL( points.count(), nPoints );

  mcc::Point defaultPoint;
  for (mcc::IPointVector::size_type i = 0; i < nPoints; ++i) {
    BOOST_CHECK_EQUAL( points[i] , defaultPoint );
    BOOST_CHECK_EQUAL( points[i].classification() , mcc::NotYetClassified );
  }
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( iterators )
{
  const mcc::IPointVector::size_type nPoints = 7;
  mcc::PointVector points(nPoints);

  // Set coordinates of each point to its index in vector, i.e.,
  // (0, 0, 0),  (1, 1, 1),  (2, 2, 2),  ...
  for (mcc::IPointVector::size_type i = 0; i < nPoints; ++i) {
    mcc::XyzPoint xyzPoint(i, i, i);
    points[i].setCoordinates(xyzPoint);
  }

  // Does IPointVector only iterate through unclassified points
  const mcc::IPointVector & constPtVec = points;
  mcc::IPointVector::size_type nPasses = 0;
  BOOST_FOREACH(const mcc::IPoint & point, constPtVec) {
    BOOST_CHECK( nPasses < nPoints );
    BOOST_CHECK_EQUAL( point.x() , nPasses );
    nPasses++;
  }
  BOOST_CHECK_EQUAL( nPasses, nPoints );
}

BOOST_AUTO_TEST_SUITE_END()
