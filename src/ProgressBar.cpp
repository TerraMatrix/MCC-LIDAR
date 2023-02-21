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

#include "ProgressBar.h"

namespace mcc
{
	//   ProgressBar::ProgressBar(std::ostream & stream,
	//                            double         workToBeDone)
	//     : stream_(stream),
	//       workToBeDone_(workToBeDone)
	//   {
	//     stream_ << "0%" << std::flush;
	//     percentDisplayed_ = 0;
	//   }
	ProgressBar::ProgressBar(progress_func func,
		int iterTimes)
		: func_(func),
		iterTimes_(iterTimes),
		curIterTimes_(-1)
	{
		// stream_ << "0%" << std::flush;
		percentDisplayed_ = 0;
	}
	//---------------------------------------------------------------------------

	void ProgressBar::updateProgress(double workDone)
	{
		if (curIterTimes_ == -1)
		{
			return;
		}
		float per = float(curIterTimes_) / iterTimes_ + float(workDone) / workToBeDone_ / iterTimes_;
		if (per > 0.99)
		{
			per = 0.99;
		}

		if (func_)
		{
			func_(per);
		}
	}


	void ProgressBar::updateWork(double workToBeDone)
	{
		curIterTimes_++;
		workToBeDone_ = workToBeDone;
	}
}
