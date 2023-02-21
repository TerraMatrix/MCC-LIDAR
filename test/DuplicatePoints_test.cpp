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

#include <cstdlib>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "DuplicatePoints.h"
#include "IPoint.h"
#include "Point.h"
#include "XyzPoint.h"

//-----------------------------------------------------------------------------

struct DuplicatePointSet
{
  std::vector<mcc::Point> points;
  mcc::Point * selectedPoint;
  mcc::PointClassification classification;  // assigned to selected Point

  DuplicatePointSet(mcc::Coordinate x,
                    mcc::Coordinate y,
                    mcc::Coordinate z,
                    int             nPointsInSet)
    : points(), selectedPoint(0)
  {
    mcc::XyzPoint xyzPoint(x, y, z);
    mcc::Point point;
    point.setCoordinates(xyzPoint);
    for (; nPointsInSet > 0; nPointsInSet--)
      points.push_back(point);
  }

  // Add pointer to the set's points to a list of unclassified points
  void addPointsToList(std::vector<mcc::IPoint *> & unclassifiedPoints)
  {
    for (std::vector<mcc::Point>::size_type i = 0; i < points.size(); ++i) {
      mcc::IPoint * point = &( points[i] );
      unclassifiedPoints.push_back(point);
    }
  }
};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE( DuplicatePoints )

BOOST_AUTO_TEST_CASE( threeSets )
{
  std::vector<DuplicatePointSet> sets;
  sets.push_back( DuplicatePointSet( 11,  22,  33, 2) );
  sets.push_back( DuplicatePointSet( 50,  60,  70, 3) );
  sets.push_back( DuplicatePointSet(409, 409, 409, 2) );

  std::vector<mcc::IPoint *> unclassifiedDuplicates;
  BOOST_FOREACH( DuplicatePointSet & set , sets ) {
    set.addPointsToList(unclassifiedDuplicates);
  }

  mcc::DuplicatePoints duplicatePts(unclassifiedDuplicates);
  BOOST_REQUIRE_EQUAL( duplicatePts.setCount(), 3 );

  int nPutAside = duplicatePts.putAsideAllButOnePointPerSet();
  BOOST_REQUIRE_EQUAL( nPutAside, 4);

  // Check that each set as just one unclassified, and the rest are DuplicatePoint
  BOOST_FOREACH( DuplicatePointSet & set, sets ) {
    int nNotYetClassified = 0;
    int nDuplicate = 0;
    BOOST_FOREACH( mcc::Point & point, set.points ) {
      switch (point.classification()) {
        case mcc::NotYetClassified:
          nNotYetClassified++;
          set.selectedPoint = &(point);
          break;

        case mcc::Duplicate:
          nDuplicate++;
          break;

        default:
          BOOST_FAIL("point.Classification() is not NotYetClassified or Duplicate");
          break;
      }
    }
    BOOST_REQUIRE_EQUAL( nNotYetClassified , 1 );
    BOOST_REQUIRE_EQUAL( nDuplicate , (int) set.points.size() - 1 );
  }

  // Set the classification code for each set's selected point (which would be
  // done by MCC algorithm).
  BOOST_FOREACH( DuplicatePointSet & set, sets ) {
    set.classification = (std::rand() % 2 == 1) ? mcc::NonGround : mcc::Ground;
    set.selectedPoint->classifyAs(set.classification);
  }

  duplicatePts.copyClassificationAmongPointsInSet();
  BOOST_FOREACH( const DuplicatePointSet & set, sets ) {
    BOOST_FOREACH( const mcc::Point & point, set.points ) {
      if (mcc::isGround(set.classification))
        BOOST_REQUIRE( mcc::isGround(point.classification()) );
      else
        BOOST_REQUIRE_EQUAL( point.classification() , set.classification );
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
