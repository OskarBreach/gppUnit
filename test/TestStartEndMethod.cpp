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
#include "src/Notification.h"
#include "src/MethodNames.h"
#include "src/MethodDescription.h"
#include "src/ReportResult.h"
#include "src/TestResult.h"

#include "TestUtilities.h"

#include <sstream>

namespace TestStartEndMethod {
	using gppUnit::equals;

	class MockTestCaseVariableResults: public Utilities::MockTestCase {
		int number;
		void test() {
			gppUnit::TestResult result;
			for(int i = 0; i < number; ++i) {
				reporter->Report(result);
			}
		}
	public:
		explicit MockTestCaseVariableResults(int n): number(n) {}
	};

	std::string setuptestteardownString(int results) {
		std::stringstream strm;
		strm << gppUnit::setupMethodName() << ".0.end." <<
		     gppUnit::testMethodName() << '.' << results << ".end." <<
		     gppUnit::teardownMethodName() << ".0.end.";
		return strm.str();
	}

	class TestNumResultsBase: public Utilities::TestCaseCaller, gppUnit::Notification {
	protected:
		std::stringstream collect;
	private:
		MockTestCaseVariableResults* testcase;
		const gppUnit::MethodDescription* description;

		void StartMethod(const gppUnit::MethodDescription& desc) {
			description = &desc;
			collect << desc.name();
		}
		void EndMethod() {
			collect << '.' << description->results() << '.' << "end.";
		}
		void teardown() {
			delete testcase;
		}
	protected:
		void givenMockTestCase(int n) {
			testcase = new MockTestCaseVariableResults(n);
			add(*testcase);
			givenNotification(this);
		}
		void thenEachMethodStartedAndEndedWithNumResults(int results) {
			confirm.that(collect, equals(setuptestteardownString(results)), "Should have called three methods");
		}
		void thenResultRecordedInTest() {
			confirm.that(collect.str().find("test.0.1.end"), !equals(std::string::npos), "Test result is 0.1");
		}
	};
	template<int NUM>
	class TestNumResults: protected TestNumResultsBase {
	protected:
		void test() {
			givenMockTestCase(NUM);
			whenCalled();
			thenEachMethodStartedAndEndedWithNumResults(NUM);
		}
	};

	class TestNoResults: public TestNumResults<0> {
	public:
	} GPPUNIT_INSTANCE;

	class TestOneResult: public TestNumResults<1> {
	public:
	} GPPUNIT_INSTANCE;

	class TestTwoResults: public TestNumResults<2> {
	public:
	} GPPUNIT_INSTANCE;

	class TestResultContentTrue: public TestNumResultsBase {
		virtual void Result(const gppUnit::TestResult& result) {
			collect << '.' << result.result;
		}
		void test() {
			givenMockTestCase(1);
			whenCalled();
			thenResultRecordedInTest();
		}
	} GPPUNIT_INSTANCE;
}
