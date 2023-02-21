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

#include <iostream>
#include <boost/test/floating_point_comparison.hpp>
#include "CoordinateTolerance.h"
#include "IPoint.h"
#include "TestUtil.h"

namespace mcc
{
  bool areCoordinatesClose(Coordinate coord1,
                           Coordinate coord2)
  {
    using namespace boost::test_tools;
    return check_is_close(coord1, coord2, percent_tolerance(Coordinate_tolerance));
  }

  bool operator==(const IPoint & lhs,
                  const IPoint & rhs)
  {
    return areCoordinatesClose(lhs.x(), rhs.x()) &&
           areCoordinatesClose(lhs.y(), rhs.y()) &&
           areCoordinatesClose(lhs.z(), rhs.z());
  }

  bool sameCoordinates(const IPoint &   point,
                       const XyzPoint & xyzPoint)
  {
    return (point.x() == xyzPoint.x) &&
           (point.y() == xyzPoint.y) &&
           (point.z() == xyzPoint.z);
  }

  std::ostream & operator<<(std::ostream & stream,
                            const IPoint & point)
  {
    stream << "(" << point.x() << ", " << point.y() << ", " << point.z() << ")";
    return stream;
  }
}
