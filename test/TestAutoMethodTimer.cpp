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
#include "src/MethodTimer.h"
#include "src/AutoMethodTimer.h"
#include "src/TimeReport.h"

#include "TestUtilities.h"

#include <sstream>

namespace TestAutoMethodTimer {
	using gppUnit::equals;

	class Test: public Auto::TestCase, gppUnit::MethodCaller, gppUnit::TimeReport {
		gppUnit::AutoMethodTimer<Utilities::MockAuto<0> >* mockauto;
		gppUnit::MethodTimer* caller;
		std::stringstream strm;

		void setup() { mockauto = new gppUnit::AutoMethodTimer<Utilities::MockAuto<0> >; }
		void teardown() { delete mockauto; }
		virtual void forward() {
			strm << "fwd.";
		}
		virtual void reportTime(double /* run_time */) {
			strm << "time.";
		}

		void givenAutoMethodTimer() {
			caller = mockauto;
		}
		void whenCalled() {
			caller->timeMethod(*this, *this);
		}
		void thenMethodCalledBeforeTimeReported() {
			confirm.that("fwd.time.", equals(strm), "forward before time");
		}
		void test() {
			givenAutoMethodTimer();
			whenCalled();
			thenMethodCalledBeforeTimeReported();
		}
	} GPPUNIT_INSTANCE;
}
