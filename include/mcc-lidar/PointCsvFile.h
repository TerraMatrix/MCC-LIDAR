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

#ifndef MCC_POINT_CSV_FILE_H
#define MCC_POINT_CSV_FILE_H

#include <boost/format.hpp>

#include "IPoint.h"
#include "OutputCsvFile.h"
#include "MCCExport.h"

namespace mcc
{
	//  A text file with point data as comma-separated values.
	class MCC_LIDAR_EXPORTS PointCsvFile : public OutputCsvFile
	{
	public:
		// Write a row of values to the file:
		//
		//   X,Y,Z
		//
		// where X,Y,Z are the point's coordinates.
		void writeRow(const IPoint& point);

	protected:
		void writeHeadings();

	private:
		boost::format rowFormat_;
	};
}

#endif
