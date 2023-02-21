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

#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include "IUnclassifiedPoints.h"
#include "PointVector.h"
#include "StackedPoints.h"
#include "TestUtil.h"
#include "UnclassifiedPoints.h"
#include "XyzPoint.h"

//-----------------------------------------------------------------------------

struct Fixture
{
  std::vector<mcc::XyzPoint> xyzPoints;
  boost::shared_ptr<mcc::PointVector> points;

  void storePoint( mcc::Coordinate x,
                   mcc::Coordinate y,
                   mcc::Coordinate z)
  {
    xyzPoints.push_back( mcc::XyzPoint(x, y, z) );
  }

  void makePointVector()
  {
    std::random_shuffle(xyzPoints.begin(), xyzPoints.end());

    const mcc::IPointVector::size_type nPoints = xyzPoints.size();
    points = boost::make_shared<mcc::PointVector>(nPoints);
    for (mcc::IPointVector::size_type i = 0; i < nPoints; ++i) {
      (*points)[i].setCoordinates(xyzPoints[i]);
    }
  }
};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE( StackedPoints )

// This test illustrates the bug described in ticket #34
BOOST_FIXTURE_TEST_CASE( duplicatePoints, Fixture )
{
  storePoint( 0   ,  1  , 0    );
  storePoint( 0.1 ,  1  , 0.1  );

  storePoint( 5   , 22  , 2.99 );
  storePoint( 5   , 22  , 2.99 );  // duplicate
  mcc::XyzPoint duplicatePoint = xyzPoints.back();

  storePoint( 9   , 7.5 , 5.1  );
  storePoint( 9.1 , 7.0 , 4    );

  makePointVector();

  mcc::UnclassifiedPoints ptsToBeClassified(points);
  std::vector<mcc::IPoint *> unclassifiedDuplicates;
  mcc::StackedPoints::classifyPointsAtSameXY(ptsToBeClassified, unclassifiedDuplicates);

  BOOST_FOREACH( mcc::IPoint & point , *points ) {
    BOOST_CHECK_EQUAL( point.classification() , mcc::NotYetClassified );
  }

  std::vector<mcc::IPoint *>::size_type expectedCount = 2;
  BOOST_CHECK_EQUAL( unclassifiedDuplicates.size() , expectedCount );
  BOOST_FOREACH( mcc::IPoint * point, unclassifiedDuplicates ) {
    BOOST_CHECK( mcc::sameCoordinates(*point, duplicatePoint) ); 
  }
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( duplicateNonGroundPoints, Fixture )
{
  storePoint( 0   ,  1  , 0    );
  storePoint( 0.1 ,  1  , 0.1  );

  storePoint( 5   , 22  , 2.99 );
  storePoint( 5   , 22  , 3    );  // non-ground
  storePoint( 5   , 22  , 3    );  // duplicate
  mcc::XyzPoint nonGroundPoint = xyzPoints.back();

  storePoint( 9   , 7.5 , 5.1  );
  storePoint( 9.1 , 7.0 , 4    );

  makePointVector();

  mcc::UnclassifiedPoints ptsToBeClassified(points);
  std::vector<mcc::IPoint *> unclassifiedDuplicates;
  mcc::StackedPoints::classifyPointsAtSameXY(ptsToBeClassified, unclassifiedDuplicates);

  BOOST_FOREACH( mcc::IPoint & point , *points ) {
    mcc::PointClassification expectedClassification;
    if (mcc::sameCoordinates(point, nonGroundPoint))
      expectedClassification = mcc::NonGround;
    else
      expectedClassification = mcc::NotYetClassified;
    BOOST_CHECK_EQUAL( point.classification() , expectedClassification );
  }

  std::vector<mcc::IPoint *>::size_type expectedCount = 0;
  BOOST_CHECK_EQUAL( unclassifiedDuplicates.size() , expectedCount );
}

BOOST_AUTO_TEST_SUITE_END()
