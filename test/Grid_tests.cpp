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

#include <cctype>

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "Grid.h"

bool isLetter(char ch)
{
  return std::isalpha(ch) ? true : false;
}

struct CellData
{
  char ch;
  int i;    // integer code for "ch" member
  bool b;   // is the "ch" member a letter?

  CellData()
    : ch('?'), i(ch), b(isLetter(ch))
  {
  }

  CellData(char character)
    : ch(character), i(ch), b(isLetter(ch))
  {
  }
};

//-----------------------------------------------------------------------------

namespace
{
  const char * const strings[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                   "abcdefghijklmnopqrstuvwxyz",
                                   "01234567898765432101234567",
                                   "~!@#$%^&*()_+-={}|[].:;'<>",
                                   "foo, bar, qux? FOO BAR QUX" };
  int rows = sizeof(strings) / sizeof(strings[0]);
  int columns = strlen(strings[0]);
  
  mcc::XYCoordinates lowerLeftCorner(78.9, 543.21);
  double cellHeight = 14.5;
  double cellWidth  = 22;
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE( Grid )

BOOST_AUTO_TEST_CASE( function_call_operator )
{
  mcc::Grid<CellData> grid(rows, columns, lowerLeftCorner, cellHeight, cellWidth);

  // Fill the grid's cell data using the characters from the string data.
  unsigned int strRow = 0;
  BOOST_FOREACH(unsigned int row, grid.topToBottom()) {
    BOOST_FOREACH(unsigned int column, grid.leftToRight()) {
      grid(row, column) = CellData(strings[strRow][column]);
    }
    ++strRow;
  }

  // Check the grid's cell data.
  const mcc::Grid<CellData> & const_grid = grid;
  strRow = 0;
  BOOST_FOREACH(unsigned int row, const_grid.topToBottom()) {
    BOOST_FOREACH(unsigned int column, const_grid.leftToRight()) {
      const CellData & cellData = const_grid(row, column);
      char expectedChar = strings[strRow][column];
      BOOST_CHECK_EQUAL( cellData.ch, expectedChar );
      BOOST_CHECK_EQUAL( cellData.i,  int(expectedChar) );
      BOOST_CHECK_EQUAL( cellData.b,  isLetter(expectedChar) );
    }
    ++strRow;
  }
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( subscript_operator )
{
  mcc::Grid<CellData> grid(rows, columns, lowerLeftCorner, cellHeight, cellWidth);

  // Fill the grid's cell data using the characters from the string data.
  unsigned int strRow = 0;
  BOOST_FOREACH(unsigned int row, grid.topToBottom()) {
    BOOST_FOREACH(unsigned int column, grid.leftToRight()) {
      mcc::Cell cell = grid.getCell(row, column);
      grid[cell] = CellData(strings[strRow][column]);
    }
    ++strRow;
  }

  // Check the grid's cell data.
  const mcc::Grid<CellData> & const_grid = grid;
  strRow = 0;
  BOOST_FOREACH(unsigned int row, const_grid.topToBottom()) {
    BOOST_FOREACH(unsigned int column, const_grid.leftToRight()) {
      mcc::Cell cell = grid.getCell(row, column);
      const CellData & cellData = const_grid[cell];
      char expectedChar = strings[strRow][column];
      BOOST_CHECK_EQUAL( cellData.ch, expectedChar );
      BOOST_CHECK_EQUAL( cellData.i,  int(expectedChar) );
      BOOST_CHECK_EQUAL( cellData.b,  isLetter(expectedChar) );
    }
    ++strRow;
  }
}

BOOST_AUTO_TEST_SUITE_END()
