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
#include "XyzPoint.h"

BOOST_AUTO_TEST_SUITE( XyzPoint )

BOOST_AUTO_TEST_CASE( default_constructor )
{
  mcc::XyzPoint point;
  // Use CHECK_SMALL because expected coordinates are zero.
  BOOST_CHECK_SMALL( point.x, mcc::Coordinate(Coordinate_tolerance) );
  BOOST_CHECK_SMALL( point.y, mcc::Coordinate(Coordinate_tolerance) );
  BOOST_CHECK_SMALL( point.z, mcc::Coordinate(Coordinate_tolerance) );
}

BOOST_AUTO_TEST_CASE( xyz_constructor )
{
  const mcc::Coordinate x = 123.456;
  const mcc::Coordinate y = -0.00789;
  const mcc::Coordinate z = +5e13;
  mcc::XyzPoint point(x, y, z);
  BOOST_CHECK_CLOSE( point.x, x, Coordinate_tolerance );
  BOOST_CHECK_CLOSE( point.y, y, Coordinate_tolerance );
  BOOST_CHECK_CLOSE( point.z, z, Coordinate_tolerance );
}

BOOST_AUTO_TEST_SUITE_END()
