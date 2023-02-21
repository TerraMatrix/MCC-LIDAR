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

#include "PointCanopyHeightFile.h"
#include "PointIO.h"

namespace
{
  std::string rowFmtSpec()
  {
    // Use coordinate format for all 4 columns: X, Y, Z, canopy height
    boost::format spec("%1%,%1%,%1%,%1%");
    spec % mcc::coordinateFmtSpec();
    return spec.str();
  }

  boost::format rowFmt(rowFmtSpec());
}

namespace mcc
{
  void PointCanopyHeightFile::writeHeadings()
  {
    stream() << "X,Y,Z,canopy height" << std::endl;
    rowFormat_ = rowFmt;
  }

  //---------------------------------------------------------------------------

  void PointCanopyHeightFile::writeRow(const IPoint & point,
                                       Coordinate     canopyHeight)
  {
    stream() << rowFormat_ % point.x() % point.y() % point.z() % canopyHeight << std::endl;
  }
}
