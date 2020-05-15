/*
Copyright (c) 2011 Andrew Wall

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef TESTMETHODTIMERS_H_5119E4AA_7A61_49FF_80C1_048CC165F549
#define TESTMETHODTIMERS_H_5119E4AA_7A61_49FF_80C1_048CC165F549

#include "src/MethodTimer.h"
#include "src/MethodCaller.h"
#include "src/TimeReport.h"

#include "AutoRun.h"

#include <sstream>

namespace TestMethodTimers {
	class MethodTimerTestHelper: public Auto::TestCase, gppUnit::MethodCaller, gppUnit::TimeReport {
		gppUnit::MethodTimer* aTimer;

	protected:
		bool methodCalled;
		bool timeReported;
		double runtimeReport;

		std::stringstream methodCallLog;

		void forward();
		void reportTime(double run_time);
		void givenTimer(gppUnit::MethodTimer& given);
		void whenCalled();
		void thenMethodCalled();
	};
}
#endif // TESTMETHODTIMERS_H_5119E4AA_7A61_49FF_80C1_048CC165F549
