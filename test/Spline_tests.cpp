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

#include <cmath>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "tpsdemo/spline.h"
#include "Geometry3D.h"
#include "XyzPoint.h"


namespace {
  struct Fixture
  {
    // Control points listed in file "edstestout.txt" attached to ticket #19
    std::vector<mcc::XyzPoint> edstestout_points;

    // Control points listed in file "muckout.txt" attached to ticket #19
    std::vector<mcc::XyzPoint> muckout_points;

    // Control points listed in file "306vesrion1.txt" attached to ticket #28
    std::vector<mcc::XyzPoint> ticket28_points;

    // Control points listed in the screenshot attached to ticket #28
    std::vector<mcc::XyzPoint> ticket30_points;

    Fixture()
      :  edstestout_points(3),
         muckout_points(3),
         ticket28_points(4),
         ticket30_points(12)
    {
      edstestout_points[0] = mcc::XyzPoint(1782230.25, 917017.75, 6462.76);
      edstestout_points[1] = mcc::XyzPoint(1782230.25, 917017.39, 6462.53);
      edstestout_points[2] = mcc::XyzPoint(1782230.25, 917017.72, 6449.34);

      muckout_points[0] = mcc::XyzPoint(7831716.77, 711636.12, 1046.88);
      muckout_points[1] = mcc::XyzPoint(7831716.77, 711635.53, 1063.48);
      muckout_points[2] = mcc::XyzPoint(7831716.77, 711635.56, 1046.72);

      ticket28_points[0] = mcc::XyzPoint(1774653.81, 887135.86, 5294.26);
      ticket28_points[1] = mcc::XyzPoint(1774653.81, 887154.46, 5293.83);
      ticket28_points[2] = mcc::XyzPoint(1774653.81, 887124.71, 5294.85);
      ticket28_points[3] = mcc::XyzPoint(1774653.81, 887147.97, 5293.93);

      ticket30_points[0]  = mcc::XyzPoint(524341.72, 5189006.49, 867.63);
      ticket30_points[1]  = mcc::XyzPoint(524341.72, 5189004.93, 867.71);
      ticket30_points[2]  = mcc::XyzPoint(524341.72, 5189006.25, 867.65);
      ticket30_points[3]  = mcc::XyzPoint(524341.72, 5189005.5, 867.65);
      ticket30_points[4]  = mcc::XyzPoint(524341.72, 5189004.32, 867.77);
      ticket30_points[5]  = mcc::XyzPoint(524341.72, 5189006.71, 867.62);
      ticket30_points[6]  = mcc::XyzPoint(524341.72, 5189005.22, 867.68);
      ticket30_points[7]  = mcc::XyzPoint(524341.72, 5189006.93, 867.58);
      ticket30_points[8]  = mcc::XyzPoint(524341.72, 5189004.62, 867.76);
      ticket30_points[9]  = mcc::XyzPoint(524341.72, 5189007.12, 867.57);
      ticket30_points[10] = mcc::XyzPoint(524341.72, 5189006.01, 867.65);
      ticket30_points[11] = mcc::XyzPoint(524341.72, 5189005.76, 867.66);
    }

    //  Fill in a vector of spline control points from a vector of MCC-LIDAR
    //  XYZ points.
    void makeControlPoints(std::vector<Vec> &                 controlPoints,
                           const std::vector<mcc::XyzPoint> & xyzPoints)
    {
      controlPoints.resize(xyzPoints.size());
      for (std::vector<Vec>::size_type i = 0; i < controlPoints.size(); ++i) {
        const mcc::XyzPoint & xyzPoint = xyzPoints[i];
        // NOTE: TPSDemo uses the y coordinate as the height, while MCC-LIDAR
        // uses the z coordinate as height.  So swap the y and z coordinates
        // when making a TPSDemo control point.
        controlPoints[i] = Vec(xyzPoint.x, xyzPoint.z, xyzPoint.y);
      }
    }

    //  Switch the X and Y coordinates for each point in a list.
    void switchXAndY(std::vector<mcc::XyzPoint> & xyzPoints)
    {
      BOOST_FOREACH(mcc::XyzPoint & point, xyzPoints) {
        double saveY = point.y;
        point.y = point.x;
        point.x = saveY;
      }
    }

    //  For each point in a list, set its X coordinate equal to its Y
    //  coordinate.
    void setXToY(std::vector<mcc::XyzPoint> & xyzPoints)
    {
      BOOST_FOREACH(mcc::XyzPoint & point, xyzPoints) {
        point.x = point.y;
      }
    }

    //  Slightly change a point's coordinate by a small amount.
    void nudgeCoordinate(mcc::Coordinate & coordinate)
    {
      coordinate += 0.0001;
    }

    //  Is the plane defined by 3 control points vertical?
    bool isPlaneVertical(const std::vector<Vec> & points)
    {
      Vec normalToPlane = mcc::Geometry3D::computeNormalToPlane(points[0], points[1], points[2]);

      // If normal has no height component ('y' in TPSDemo coordinates), so it
      // is horizontal, and the plane is vertical.
      return std::abs(normalToPlane.y) <= 0.00000000001;
    }

    //  Is a list of points collinear when projected onto the X,Y plane?
    bool areCollinearInXY(const std::vector<mcc::XyzPoint> & xyzPoints)
    {
      std::vector<Vec> points(xyzPoints.size());
      for (std::vector<Vec>::size_type i = 0; i < points.size(); ++i) {
        const mcc::XyzPoint & xyzPoint = xyzPoints[i];
        points[i] = Vec(xyzPoint.x, xyzPoint.y, xyzPoint.z);
      }
      return mcc::Geometry3D::areCollinearInXY(points);
    }
  };
}


BOOST_FIXTURE_TEST_SUITE( Spline , Fixture )

//-----------------------------------------------------------------------------

//  Reproduce the singular matrix error that was reported in ticket #19
//  (control points listed in attachment "edstestout.txt")
BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_edstestout )
{
  std::vector<Vec> control_points;
  makeControlPoints(control_points, edstestout_points);

  BOOST_CHECK( isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_THROW( tpsdemo::Spline spline(control_points, 0.0),
                       tpsdemo::SingularMatrixError );
}


//  Reproduce the singular matrix error with modified control points from the
//  "edstestout.txt" attachment.  The points' X and Y coordinates are switched
//  so the points now lie in an east-west direction (same Y coordinate).
BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_edstestout_XAndYSwitched )
{
  std::vector<mcc::XyzPoint> edstestout_points_XAndYSwitched(edstestout_points);
  switchXAndY(edstestout_points_XAndYSwitched);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, edstestout_points_XAndYSwitched);

  BOOST_CHECK( isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_THROW( tpsdemo::Spline spline(control_points, 0.0),
                       tpsdemo::SingularMatrixError );
}


//  Reproduce the singular matrix error with modified control points from the
//  "edstestout.txt" attachment.  The points' X coordinates are set to their Y
//  coordinates so the points now lie in a plane running diagonally from
//  southwest to northeast (y = x).
BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_edstestout_XSetToY )
{
  std::vector<mcc::XyzPoint> edstestout_points_XSetToY(edstestout_points);
  setXToY(edstestout_points_XSetToY);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, edstestout_points_XSetToY);

  BOOST_CHECK( isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_THROW( tpsdemo::Spline spline(control_points, 0.0),
                       tpsdemo::SingularMatrixError );
}

//-----------------------------------------------------------------------------

//  Tests with the control points from the "edstestout.txt" file attached to
//  ticket #19.  Each test tweaks one control point, nudging it from its
//  position by changing one of its coordinates ever so slightly, in order to
//  prevent the singular-matrix error.

BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_edstestout_nudged )
{
  std::vector<mcc::XyzPoint> edstestout_points_pointNudged(edstestout_points);
  nudgeCoordinate(edstestout_points_pointNudged[0].x);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, edstestout_points_pointNudged);

  BOOST_CHECK( ! isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_NO_THROW( tpsdemo::Spline spline(control_points, 0.0) );
}


BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_edstestout_XAndYSwitched_pointNudged )
{
  std::vector<mcc::XyzPoint> edstestout_points_XAndYSwitched_pointNudged(edstestout_points);
  switchXAndY(edstestout_points_XAndYSwitched_pointNudged);
  nudgeCoordinate(edstestout_points_XAndYSwitched_pointNudged[0].y);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, edstestout_points_XAndYSwitched_pointNudged);

  BOOST_CHECK( ! isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_NO_THROW( tpsdemo::Spline spline(control_points, 0.0) );
}


BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_edstestout_XSetToY_pointNudged )
{
  std::vector<mcc::XyzPoint> edstestout_points_XSetToY_pointNudged(edstestout_points);
  setXToY(edstestout_points_XSetToY_pointNudged);
  nudgeCoordinate(edstestout_points_XSetToY_pointNudged[0].y);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, edstestout_points_XSetToY_pointNudged);

  BOOST_CHECK( ! isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_NO_THROW( tpsdemo::Spline spline(control_points, 0.0) );
}

//-----------------------------------------------------------------------------

//  Reproduce the singular matrix error that was reported in ticket #19
//  (control points listed in attachment "muckout.txt")
BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_muckout )
{
  std::vector<Vec> control_points;
  makeControlPoints(control_points, muckout_points);

  BOOST_CHECK( isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_THROW( tpsdemo::Spline spline(control_points, 0.0),
                       tpsdemo::SingularMatrixError );
}


//  Reproduce the singular matrix error with modified control points from the
//  "muckout.txt" attachment.  The points' X and Y coordinates are switched
//  so the points now lie in an east-west direction (same Y coordinate).
BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_muckout_XAndYSwitched )
{
  std::vector<mcc::XyzPoint> muckout_points_XAndYSwitched(muckout_points);
  switchXAndY(muckout_points_XAndYSwitched);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, muckout_points_XAndYSwitched);

  BOOST_CHECK( isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_THROW( tpsdemo::Spline spline(control_points, 0.0),
                       tpsdemo::SingularMatrixError );
}


//  Reproduce the singular matrix error with modified control points from the
//  "muckout.txt" attachment.  The points' X coordinates are set to their Y
//  coordinates so the points now lie in a plane running diagonally from
//  southwest to northeast (y = x).
BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_muckout_XSetToY )
{
  std::vector<mcc::XyzPoint> muckout_points_XSetToY(muckout_points);
  setXToY(muckout_points_XSetToY);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, muckout_points_XSetToY);

  BOOST_CHECK( isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_THROW( tpsdemo::Spline spline(control_points, 0.0),
                       tpsdemo::SingularMatrixError );
}

//-----------------------------------------------------------------------------

//  Tests with the control points from the "muckout.txt" file attached to
//  ticket #19.  Each test tweaks one control point, nudging it from its
//  position by changing one of its coordinates ever so slightly, in order to
//  prevent the singular-matrix error.

BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_muckout_pointNudged )
{
  std::vector<mcc::XyzPoint> muckout_points_pointNudged(muckout_points);
  nudgeCoordinate(muckout_points_pointNudged[0].x);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, muckout_points_pointNudged);

  BOOST_CHECK( ! isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_NO_THROW( tpsdemo::Spline spline(control_points, 0.0) );
}


BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_muckout_XAndYSwitched_pointNudged )
{
  std::vector<mcc::XyzPoint> muckout_points_XAndYSwitched_pointNudged(muckout_points);
  switchXAndY(muckout_points_XAndYSwitched_pointNudged);
  nudgeCoordinate(muckout_points_XAndYSwitched_pointNudged[0].y);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, muckout_points_XAndYSwitched_pointNudged);

  BOOST_CHECK( ! isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_NO_THROW( tpsdemo::Spline spline(control_points, 0.0) );
}


BOOST_AUTO_TEST_CASE( singular_matrix_ticket19_muckout_XSetToY_pointNudged )
{
  std::vector<mcc::XyzPoint> muckout_points_XSetToY_pointNudged(muckout_points);
  setXToY(muckout_points_XSetToY_pointNudged);
  nudgeCoordinate(muckout_points_XSetToY_pointNudged[0].y);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, muckout_points_XSetToY_pointNudged);

  BOOST_CHECK( ! isPlaneVertical(control_points) );
  
  BOOST_REQUIRE_NO_THROW( tpsdemo::Spline spline(control_points, 0.0) );
}

//-----------------------------------------------------------------------------

//  Reproduce the singular matrix error that was reported in ticket #28
BOOST_AUTO_TEST_CASE( singular_matrix_ticket28 )
{
  std::vector<Vec> control_points;
  makeControlPoints(control_points, ticket28_points);

  BOOST_CHECK( areCollinearInXY(ticket28_points) );
  
  BOOST_REQUIRE_THROW( tpsdemo::Spline spline(control_points, 0.0),
                       tpsdemo::SingularMatrixError );
}

//  Just nudge one point among the control points reported for ticket #28 to
//  prevent the singular matrix error.
BOOST_AUTO_TEST_CASE( singular_matrix_ticket28_pointNudged )
{
  std::vector<mcc::XyzPoint> ticket28_points_pointNudged(ticket28_points);
  nudgeCoordinate(ticket28_points_pointNudged[0].x);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, ticket28_points_pointNudged);

  BOOST_CHECK( ! areCollinearInXY(ticket28_points_pointNudged) );
  
  BOOST_REQUIRE_NO_THROW( tpsdemo::Spline spline(control_points, 0.0) );
}

//-----------------------------------------------------------------------------

//  Reproduce the singular matrix error that was reported in ticket #30
BOOST_AUTO_TEST_CASE( singular_matrix_ticket30 )
{
  std::vector<Vec> control_points;
  makeControlPoints(control_points, ticket30_points);

  BOOST_CHECK( areCollinearInXY(ticket30_points) );
  
  BOOST_REQUIRE_THROW( tpsdemo::Spline spline(control_points, 0.0),
                       tpsdemo::SingularMatrixError );
}

//  Just nudge one point among the control points reported for ticket #30 to
//  prevent the singular matrix error.
BOOST_AUTO_TEST_CASE( singular_matrix_ticket30_pointNudged )
{
  std::vector<mcc::XyzPoint> ticket30_points_pointNudged(ticket30_points);
  nudgeCoordinate(ticket30_points_pointNudged[7].x);

  std::vector<Vec> control_points;
  makeControlPoints(control_points, ticket30_points_pointNudged);

  BOOST_CHECK( ! areCollinearInXY(ticket30_points_pointNudged) );
  
  BOOST_REQUIRE_NO_THROW( tpsdemo::Spline spline(control_points, 0.0) );
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
