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

#include <boost/test/unit_test.hpp>

#include "CoordinateTolerance.h"
#include "IPoint.h"
#include "Point.h"
#include "XyzPoint.h"

struct Fixture
{
  mcc::Coordinate x;
  mcc::Coordinate y;
  mcc::Coordinate z;
  mcc::XyzPoint   xyzPoint;

  Fixture()
    : x(123.456), y(-0.00789), z(+5e13), xyzPoint(x, y, z)
  { }
};

BOOST_AUTO_TEST_SUITE( Point )

BOOST_AUTO_TEST_CASE( constructor )
{
  mcc::Point point;

  // Use CHECK_SMALL because expected coordinates are zero.
  BOOST_CHECK_SMALL( point.x(), mcc::Coordinate(Coordinate_tolerance) );
  BOOST_CHECK_SMALL( point.y(), mcc::Coordinate(Coordinate_tolerance) );
  BOOST_CHECK_SMALL( point.z(), mcc::Coordinate(Coordinate_tolerance) );

  BOOST_CHECK_EQUAL( point.classification(), mcc::NotYetClassified );
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( setCoordinates, Fixture )
{
  mcc::Point point;
  point.setCoordinates(xyzPoint);
  BOOST_CHECK_CLOSE( point.x(), x, Coordinate_tolerance );
  BOOST_CHECK_CLOSE( point.y(), y, Coordinate_tolerance );
  BOOST_CHECK_CLOSE( point.z(), z, Coordinate_tolerance );

  BOOST_CHECK_EQUAL( point.classification(), mcc::NotYetClassified );
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( classifyGround, Fixture )
{
  mcc::Point point;
  BOOST_CHECK_EQUAL( point.classification(), mcc::NotYetClassified );

  mcc::IPoint & iPoint = point;
  iPoint.classifyAs(mcc::Ground);
  BOOST_CHECK_EQUAL( point.classification(), mcc::Ground );
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( asIPoint, Fixture )
{
  mcc::Point point;
  point.setCoordinates(xyzPoint);

  mcc::IPoint & iPoint = point;
  BOOST_CHECK_CLOSE( iPoint.x(), x, Coordinate_tolerance );
  BOOST_CHECK_CLOSE( iPoint.y(), y, Coordinate_tolerance );
  BOOST_CHECK_CLOSE( iPoint.z(), z, Coordinate_tolerance );

  BOOST_CHECK_EQUAL( point.classification(), mcc::NotYetClassified );

  iPoint.classifyAs(mcc::NonGround);
  BOOST_CHECK_EQUAL( point.classification(), mcc::NonGround );
}

BOOST_AUTO_TEST_SUITE_END()
