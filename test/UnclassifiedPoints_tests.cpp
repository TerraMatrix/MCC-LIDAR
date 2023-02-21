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
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include "PointVector.h"
#include "TestUtil.h"
#include "UnclassifiedPoints.h"

BOOST_AUTO_TEST_SUITE( UnclassifiedPoints )

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( removeClassified )
{
  const mcc::IPointVector::size_type nPoints = 7;
  boost::shared_ptr<mcc::PointVector> points = boost::make_shared<mcc::PointVector>(nPoints);

  // Set coordinates of each point to its index in vector, i.e.,
  // (0, 0, 0),  (1, 1, 1),  (2, 2, 2),  ...
  for (mcc::IPointVector::size_type i = 0; i < nPoints; ++i) {
    mcc::XyzPoint xyzPoint(i, i, i);
    (*points)[i].setCoordinates(xyzPoint);
  }

  // Iterate with IUnclassifiedPoints interface and classify points (1, 1, 1) and
  // (4, 4, 4) as non-ground
  mcc::UnclassifiedPoints ptsToBeClassified(points);
  mcc::IUnclassifiedPoints & unclassifiedPts = ptsToBeClassified;

  mcc::IPointVector::size_type nClassified = 0;
  BOOST_FOREACH(mcc::IPoint & point, unclassifiedPts) {
    if ((point.x() == 1) || (point.x() == 4)) {
      point.classifyAs(mcc::NonGround);
      nClassified++;
    }
  }

  // Remove the classified points
  BOOST_CHECK_EQUAL( unclassifiedPts.removeClassified(), nClassified );
  mcc::IPointVector::size_type nPointsAfterRemove = nPoints - nClassified;
  BOOST_CHECK_EQUAL( unclassifiedPts.count(), nPointsAfterRemove );

  // Are all the points still in the PointVector?
  for (mcc::IPointVector::size_type i = 0; i < nPoints; ++i) {
    mcc::XyzPoint xyzPoint(i, i, i);
    mcc::Point expectedPoint;
    expectedPoint.setCoordinates(xyzPoint);
    BOOST_CHECK_EQUAL( (*points)[i], expectedPoint );

    mcc::PointClassification expectedClassification;
    if ((i == 1) || (i == 4))
      expectedClassification = mcc::NonGround;
    else
      expectedClassification = mcc::NotYetClassified;
    BOOST_CHECK_EQUAL( (*points)[i].classification(), expectedClassification );
  }

  // Does IUnclassifiedPoints only iterate through unclassified points
  const mcc::IUnclassifiedPoints & constUnclassifiedPts = ptsToBeClassified;
  mcc::IPointVector::size_type nPasses = 0;
  BOOST_FOREACH(const mcc::IPoint & point, constUnclassifiedPts) {
    BOOST_CHECK( nPasses < nPointsAfterRemove );
    if ((point.x() == 1) || (point.x() == 4)) {
      std::ostringstream message;
      message << "Iterator returned classified point: " << point;
      BOOST_ERROR( message.str() );
    }
    nPasses++;
  }
  BOOST_CHECK_EQUAL( nPasses, nPointsAfterRemove );
}

BOOST_AUTO_TEST_SUITE_END()
