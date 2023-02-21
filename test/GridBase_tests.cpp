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

#include <cmath>

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "CoordinateTolerance.h"
#include "GridBase.h"

namespace {
  struct Fixture
  {
    const unsigned int rows;
    const unsigned int columns;
    const mcc::Coordinate x0;
    const mcc::Coordinate y0;
    const double cellHeight;
    const double cellWidth;
    mcc::GridBase gridBase;

    Fixture()
      : rows(216), columns(634),
        x0(521728.25), y0(5188225),
        cellHeight(5.958333333333333), cellWidth(5.965299684542587),
        gridBase(rows, columns, mcc::XYCoordinates(x0, y0), cellHeight, cellWidth)
    {
    }

    //-------------------------------------------------------------------------

    void checkCell(const mcc::Cell & cell,
                   unsigned int      row,
                   unsigned int      column) const
    {
      BOOST_CHECK_EQUAL( cell.row(), row );
      BOOST_CHECK_EQUAL( cell.column(), column );
      mcc::XYCoordinates expectedCenter(x0 + cellWidth /2 + (column * cellWidth),
                                        y0 + cellHeight/2 + (row    * cellHeight));
      BOOST_CHECK_CLOSE( cell.x(), expectedCenter.x, Coordinate_tolerance );
      BOOST_CHECK_CLOSE( cell.y(), expectedCenter.y, Coordinate_tolerance );
    }

    //-------------------------------------------------------------------------

    unsigned int convertYToRow(mcc::Coordinate y)
    {
      return (unsigned int)(std::floor((y - y0) / cellHeight));
    }

    //-------------------------------------------------------------------------

    unsigned int convertXToColumn(mcc::Coordinate x)
    {
      return (unsigned int)(std::floor((x - x0) / cellWidth));
    }
  };
}

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE( GridBase, Fixture )

BOOST_AUTO_TEST_CASE( constructor )
{
  BOOST_CHECK_EQUAL( gridBase.rows(), rows );
  BOOST_CHECK_EQUAL( gridBase.columns(), columns );

  BOOST_CHECK_CLOSE( gridBase.lowerLeft().x, x0, Coordinate_tolerance );
  BOOST_CHECK_CLOSE( gridBase.lowerLeft().y, y0, Coordinate_tolerance );

  BOOST_CHECK_CLOSE( double(gridBase.cellHeight()), cellHeight, 0.0001 );
  BOOST_CHECK_CLOSE( double(gridBase.cellWidth()),  cellWidth,  0.0001 );
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( row_iteration )
{
  BOOST_REQUIRE( gridBase.topRow() != gridBase.bottomRow() );

  bool rowDecreasesFromTopToBottom = (gridBase.topRow() > gridBase.bottomRow());
  unsigned int nRows;
  if (rowDecreasesFromTopToBottom)
    nRows = gridBase.topRow() - gridBase.bottomRow() + 1;
  else
    nRows = gridBase.bottomRow() - gridBase.topRow() + 1;
  BOOST_REQUIRE_EQUAL( nRows, rows);

  unsigned int nPasses = 0;
  unsigned int expectedRow = gridBase.topRow();
  BOOST_FOREACH(unsigned int row, gridBase.topToBottom()) {
    ++nPasses;
    BOOST_REQUIRE( nPasses <= rows );
    BOOST_REQUIRE_EQUAL( row, expectedRow );
    if (nPasses < rows) {
      if (rowDecreasesFromTopToBottom)
        --expectedRow;
      else
        ++expectedRow;
    }
  }
  BOOST_REQUIRE_EQUAL( nPasses, rows );
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( column_iteration )
{
  BOOST_REQUIRE( gridBase.leftColumn() != gridBase.rightColumn() );

  bool columnDecreasesFromLeftToRight = (gridBase.leftColumn() > gridBase.rightColumn());
  unsigned int nColumns;
  if (columnDecreasesFromLeftToRight)
    nColumns = gridBase.leftColumn() - gridBase.rightColumn() + 1;
  else
    nColumns = gridBase.rightColumn() - gridBase.leftColumn() + 1;
  BOOST_REQUIRE_EQUAL( nColumns, columns);

  unsigned int nPasses = 0;
  unsigned int expectedColumn = gridBase.leftColumn();
  BOOST_FOREACH(unsigned int column, gridBase.leftToRight()) {
    ++nPasses;
    BOOST_REQUIRE( nPasses <= columns );
    BOOST_REQUIRE_EQUAL( column, expectedColumn );
    if (nPasses < columns) {
      if (columnDecreasesFromLeftToRight)
        --expectedColumn;
      else
        ++expectedColumn;
    }
  }
  BOOST_REQUIRE_EQUAL( nPasses, columns );
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( X_intervals )
{
  mcc::Coordinate upperBound_intervalOnLeft(x0);
  unsigned int nIntervals = 0;
  BOOST_FOREACH(unsigned int column, gridBase.leftToRight()) {
    ++nIntervals;
    BOOST_REQUIRE( nIntervals <= columns );

    mcc::CoordinateInterval xInterval = gridBase.getXInterval(column);
    // Intervals must not overlap, so this interval's lower bound must be the
    // same as the upper bound of the interval to the left.
    BOOST_REQUIRE_EQUAL( xInterval.lowerBound(), upperBound_intervalOnLeft);

    mcc::Coordinate expectedUpperBound(x0 + nIntervals*cellWidth);
    BOOST_REQUIRE_CLOSE( xInterval.upperBound(), expectedUpperBound, Coordinate_tolerance);

    if (nIntervals < columns) {
      upperBound_intervalOnLeft = xInterval.upperBound();
    }
  }
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( Y_intervals )
{
  mcc::Coordinate yMax = y0 + rows * cellHeight;
  mcc::Coordinate lowerBound_intervalAbove = yMax;
  unsigned int nIntervals = 0;
  BOOST_FOREACH(unsigned int row, gridBase.topToBottom()) {
    ++nIntervals;
    BOOST_REQUIRE( nIntervals <= rows );

    mcc::CoordinateInterval yInterval = gridBase.getYInterval(row);
    // Intervals must not overlap, so this interval's upper bound must be the
    // same as the lower bound of the interval to the left.
    BOOST_REQUIRE_EQUAL( yInterval.upperBound(), lowerBound_intervalAbove);

    mcc::Coordinate expectedLowerBound(yMax - nIntervals*cellHeight);
    BOOST_REQUIRE_CLOSE( yInterval.lowerBound(), expectedLowerBound, Coordinate_tolerance);

    if (nIntervals < rows) {
      lowerBound_intervalAbove = yInterval.lowerBound();
    }
  }
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( getCell_rowColumn_lowerLeft )
{
  // lower left cell
  unsigned int row = gridBase.bottomRow();
  unsigned int column = gridBase.leftColumn();
  checkCell(gridBase.getCell(row, column), row, column);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( getCell_rowColumn_upperLeft )
{
  // upper left cell
  unsigned int row = gridBase.topRow();
  unsigned int column = gridBase.leftColumn();
  checkCell(gridBase.getCell(row, column), row, column);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( getCell_rowColumn_upperRight )
{
  // upper right cell
  unsigned int row = gridBase.topRow();
  unsigned int column = gridBase.rightColumn();
  checkCell(gridBase.getCell(row, column), row, column);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( getCell_rowColumn_lowerRight )
{
  // lower right cell
  unsigned int row = gridBase.bottomRow();
  unsigned int column = gridBase.rightColumn();
  checkCell(gridBase.getCell(row, column), row, column);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( getCell_XY_lowerLeft )
{
  mcc::Coordinate x(x0 + 0.25*cellWidth), y(y0 + 0.4*cellWidth);
  unsigned int expectedRow    = convertYToRow(y);
  unsigned int expectedColumn = convertXToColumn(x);
  checkCell(gridBase.getCell(x, y), expectedRow, expectedColumn);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( getCell_XY_upperRight )
{
  mcc::Coordinate xMax(x0 + columns*cellWidth);
  mcc::Coordinate yMax(y0 + rows*cellHeight);
  mcc::Coordinate x(xMax - 0.1*cellWidth), y(yMax - 0.9*cellHeight);
  unsigned int expectedRow    = convertYToRow(y);
  unsigned int expectedColumn = convertXToColumn(x);
  checkCell(gridBase.getCell(x, y), expectedRow, expectedColumn);
}

BOOST_AUTO_TEST_SUITE_END()
