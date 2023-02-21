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

#ifndef MCC_COORDINATE_H
#define MCC_COORDINATE_H

namespace mcc
{
  // A single coordinate for a point.
  typedef double Coordinate;
  struct CoordinateWithEnergy
  {
    Coordinate coord;
    double Energy;
    CoordinateWithEnergy()
    {
      coord = 0.0;
      Energy = 0.0;
    }
    CoordinateWithEnergy(Coordinate c, double e)
    {
      coord = c;
      Energy = e;
    }
    bool operator==(const CoordinateWithEnergy &e) const
    {
      return coord == e.coord;
    }
  };
}

#endif
