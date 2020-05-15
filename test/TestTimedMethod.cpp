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
#include "src/MethodNames.h"
#include "src/MethodDescription.h"
#include "src/TestCaseMethodCaller.h"
#include "src/TimeReport.h"

#include "TestUtilities.h"

#include <sstream>

namespace TestTimedMethod {
	using gppUnit::equals;

	class MockTestCase: public Utilities::MockTestCase {
		void test() { }
	};

	std::string setuptestteardownString(double run_time) {
		std::stringstream strm;
		strm << gppUnit::setupMethodName() << '.' << run_time << ".end." <<
		     gppUnit::testMethodName() << '.' << run_time << ".end." <<
		     gppUnit::teardownMethodName() << '.' << run_time << ".end.";
		return strm.str();
	}

	class TestRunTime: public Utilities::TestCaseCaller, gppUnit::Notification {
		std::stringstream collect;
		MockTestCase testcase;
		gppUnit::AutoMethodTimer<Utilities::MockAuto<11> > autoTimer;
		const gppUnit::MethodDescription* description;

		void StartMethod(const gppUnit::MethodDescription& desc) {
			description = &desc;
			collect << desc.name();
		}
		void EndMethod() {
			collect << '.' << description->run_time() << '.' << "end.";
		}
		double timeToReport;
		void timeMethod(gppUnit::MethodCaller& method, gppUnit::TimeReport& report) {
			method.forward();
			report.reportTime(timeToReport);
		}

		void givenMockTestCase() {
			add(testcase);
			givenNotification(this);
		}
		void givenTimeToReport(double run_time) {
			timeToReport = run_time;
			expect.isTrue(timeToReport != 0.1, "Sample time used by test is not 0.1");
		}
		void thenEachMethodStartedAndEndedWithRunTime() {
			confirm.that(collect, equals(setuptestteardownString(timeToReport)), "Should have called three methods");
		}
		void test() {
			givenMockTestCase();
			givenTimeToReport(1.1);
			givenTimer(&autoTimer);
			whenCalled();
			thenEachMethodStartedAndEndedWithRunTime();
		}
	} GPPUNIT_INSTANCE;
}
