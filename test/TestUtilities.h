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
#ifndef TESTUTILITIES_H_B22D7F0E_4A0D_4EFB_9092_AE3C8B950C52
#define TESTUTILITIES_H_B22D7F0E_4A0D_4EFB_9092_AE3C8B950C52

#include "AutoRun.h"

#include "src/Notification.h"
#include "src/AutoMethodTimer.h"
#include "src/TestCase.h"
#include "src/ProjectContext.h"

#include <sstream>

namespace Utilities {
	void reportTimeDividedByTen(gppUnit::TimeReport& report, int Time);

	template<int TIME>
	class MockAuto {
		gppUnit::TimeReport& report;
	public:
		explicit MockAuto(gppUnit::TimeReport& report): report(report) {}
		~MockAuto() {
			reportTimeDividedByTen(report, TIME);
		}
	};

	class TestCaseCaller: public Auto::TestCase {
		gppUnit::TestCaseList cases;

		gppUnit::CommandLineOptions options;

		gppUnit::ProjectContext context;

		gppUnit::AutoMethodTimer<MockAuto<1>> autoTimer;

		bool projectReturn;
	protected:
		void add(gppUnit::PrototypeTestCase& testcase) {
			cases.push_back(&testcase);
		}

		void givenNotification(gppUnit::Notification* notified) {
			context.notify = gppUnit::CompositeNotification();
			context.notify.add(*notified);
		}
		void givenTimer(gppUnit::MethodTimer* timed) { context.timer = timed; }
		void whenCalled();
		bool projectSummary() { return projectReturn;	}

		TestCaseCaller(): cases(), context(&autoTimer, options), projectReturn(false) {}
	};
	class MockTestCase: public virtual gppUnit::PrototypeTestCase {
		void setReport(gppUnit::ReportResult* report) { reporter = report; }
	protected:
		gppUnit::ReportResult* reporter;
	};
	class ReportingMockTestCase: public MockTestCase {
		void test();
	};

	extern gppUnit::TestCaseList dummyTestCaseList;
}

#endif // TESTUTILITIES_H_B22D7F0E_4A0D_4EFB_9092_AE3C8B950C52
