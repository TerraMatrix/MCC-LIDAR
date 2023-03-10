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

#ifndef MCC_ISURFACE_INTERPOLATION_H
#define MCC_ISURFACE_INTERPOLATION_H

#include <boost/shared_ptr.hpp>

#include "MCCExport.h"

namespace mcc
{
	class IPointVector;
	class IRasterSurface;
	class ProgressBar;
	class ISurfaceInterpolation
	{
	public:
		virtual boost::shared_ptr<IRasterSurface> operator()(const IPointVector &points,
															 double cellResolution,
															 double tension,
															 ProgressBar &progress) = 0;
		virtual ~ISurfaceInterpolation() {}
	};
}

#endif
