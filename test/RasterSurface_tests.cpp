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

#include "RasterSurface.h"

namespace
{
  const unsigned int rows = 5;
  const unsigned int cols = 7;

  mcc::Coordinate values[rows][cols] = {
    { 1,  2,  3,  4,  5,  6,  7 },
    { 1,  2,  3,  4,  5,  6,  7 },
    { 9,  8,  7,  6,  5,  4,  3 },
    { 0,  2,  0,  4,  0,  6,  0 },
    {-1, 22,1.5,  8,  8,  6,  7 }
  };

  const double cellSize = 1.0;
  const mcc::Coordinate x0 = -(cellSize / 2);
  const mcc::Coordinate y0 = -(cellSize / 2);
  const mcc::XYCoordinates lowerLeft(x0, y0);

  struct Fixture
  {
    mcc::RasterSurface raster;

    Fixture()
      : raster(rows, cols, lowerLeft, cellSize)
    {
      mcc::Grid<mcc::Coordinate> & grid = raster;
      unsigned int dataRow = 0;
      BOOST_FOREACH(unsigned int row, raster.topToBottom()) {
        BOOST_FOREACH(unsigned int column, raster.leftToRight()) {
          grid(row, column) = values[dataRow][column];
        }
        ++dataRow;
      }
    }

    void checkRaster(const mcc::Coordinate expectedValues[rows][cols]) const
    {
      unsigned int dataRow = 0;
      BOOST_FOREACH(unsigned int row, raster.topToBottom()) {
        BOOST_FOREACH(unsigned int column, raster.leftToRight()) {
          mcc::Coordinate x = x0 + column*cellSize;
          mcc::Coordinate y = y0 + row*cellSize;
          BOOST_CHECK_CLOSE( raster(x, y), expectedValues[dataRow][column], 0.001 );
        }
        ++dataRow;
      }
    }
  };
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE( RasterSurface, Fixture )

BOOST_AUTO_TEST_CASE( average )
{
  raster.average(3);

  mcc::Coordinate averages[rows][cols] = {
    { 1.50000, 2.00000, 3.00000, 4.00000, 5.00000, 6.00000, 6.50000 },
    { 3.83333, 4.00000, 4.33333, 4.66667, 5.00000, 5.33333, 5.50000 },
    { 3.66667, 3.55556, 4.00000, 3.77778, 4.44444, 4.00000, 4.33333 },
    { 6.66667, 5.38889, 6.50000, 4.38889, 5.22222, 4.33333, 4.33333 },
    { 5.75000, 4.08333, 6.25000, 3.58333, 5.33333, 4.50000, 4.75000 }
  };

  checkRaster(averages);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( averageWithNoData )
{
  raster.setNoDataValue(8);

  raster.average(3);

  mcc::Coordinate averages[rows][cols] = {
    { 1.50000, 2.00000, 3.00000, 4.00000, 5.00000, 6.00000, 6.50000 },
    { 3.00000, 3.50000, 3.87500, 4.66667, 5.00000, 5.33333, 5.50000 },
    { 2.80000, 8,       3.50000, 3.77778, 4.44444, 4.00000, 4.33333 },
    { 6.40000, 5.06250, 6.07143, 3.35714, 4.42857, 3.87500, 4.33333 },
    { 5.75000, 4.08333, 5.90000, 8,       8,       3.80000, 4.75000 }
  };

  checkRaster(averages);
}

BOOST_AUTO_TEST_SUITE_END()
