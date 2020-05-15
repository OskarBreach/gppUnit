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
#include "src/TestCase.h"
#include "src/ClassDescription.h"

#include "TestUtilities.h"

namespace TestStartEndClass {
	using gppUnit::equals;

	class TestOneClass: public Utilities::TestCaseCaller, gppUnit::Notification {
		Utilities::ReportingMockTestCase testcase;

		const gppUnit::ClassDescription* classDesc;
		size_t methods;
		size_t results;
		double run_time;
	protected:
		std::stringstream collect;
		virtual void StartClass(const gppUnit::ClassDescription& desc) {
			collect << desc.name() << '.';
			classDesc = &desc;
		}
		virtual void EndClass() {
			collect << "end.";
			methods = classDesc->methods();
			results = classDesc->results();
			run_time = classDesc->run_time();
			classDesc = 0;
		}
	private:
		void givenTestCase() {
			add(testcase);
			givenNotification(this);
			methods = 0;
			results = 0;
			run_time = -1;
		}
		void thenEachClassStartedAndEnded() {
			confirm.that(collect, equals("Utilities::ReportingMockTestCase.end."), "Should have recorded test");
		}
		void thenThreeMethodsCalled() {
			confirm.that(methods, equals(3), "thenThreeMethodsCalled");
		}
		void thenOneTestResult() {
			confirm.that(results, equals(1), "thenOneTestResult");
		}
		void thenRunTimeIsThreeTimesStandard() {
			double expected = 0.3;
			bool result =
			    (expected - 0.001 < run_time) &&
			    (run_time < expected + 0.001);
			confirm.isTrue(result, "thenRunTimeIsThreeTimesStandard");
		}
		void test() {
			givenTestCase();
			whenCalled();
			thenEachClassStartedAndEnded();
			thenThreeMethodsCalled();
			thenOneTestResult();
			thenRunTimeIsThreeTimesStandard();
		}
	} GPPUNIT_INSTANCE;
}
