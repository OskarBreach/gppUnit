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
#include "src/MethodDescription.h"
#include "src/ReportResult.h"
#include "src/TestResult.h"
#include "src/AssertException.h"

#include "TestUtilities.h"

#include <stdexcept>
#include <sstream>

namespace TestExceptionCatching {
	using gppUnit::equals;

	class RTEThrowingTestCase: public Utilities::MockTestCase {
		void test() {
			throw std::runtime_error("rte");
		}
	};
	class StringThrowingTestCase: public Utilities::MockTestCase {
		void test() {
			throw std::string("string");
		}
	};
	class CharStarThrowingTestCase: public Utilities::MockTestCase {
		void test() {
			throw "charstar";
		}
	};
	class IntThrowingTestCase: public Utilities::MockTestCase {
		void test() {
			throw 1;
		}
	};
	class SelfThrowingTestCase: public Utilities::MockTestCase {
		void test() {
			throw this;
		}
	};
	class AssertExceptionThrowingTestCase: public Utilities::MockTestCase {
		void test() {
			throw gppUnit::assertException;
		}
	};

	class VariousExceptions: public Utilities::TestCaseCaller, gppUnit::Notification {
		RTEThrowingTestCase testcase1;
		StringThrowingTestCase testcase2;
		CharStarThrowingTestCase testcase3;
		IntThrowingTestCase testcase4;
		SelfThrowingTestCase testcase5;

	protected:
		std::stringstream collect;
		bool exceptionReported;
	private:

		const gppUnit::MethodDescription* description;

		double run_time;
		bool runtimeCaptured;

		void StartMethod(const gppUnit::MethodDescription& desc) {
			description = &desc;
		}

		void Exception(const std::string& what) {
			exceptionReported = true;
			collect << what << '.';
		}
		void EndMethod() {
			if(!runtimeCaptured) {
				if(description->name() == "test") {
					run_time = description->run_time();
					runtimeCaptured = true;
				}
			}
			description = 0;
		}

		virtual void givenTestCases() {
			add(testcase1);
			add(testcase2);
			add(testcase3);
			add(testcase4);
			add(testcase5);
		}
		void givenTestCase() {
			givenTestCases();
			givenNotification(this);
			exceptionReported = false;
			run_time = -1;
			runtimeCaptured = false;
		}
		virtual void thenExceptionReported() {
			confirm.isTrue(exceptionReported, "thenExceptionReported");
			confirm.that(collect, equals("rte.string.charstar.1.Unknown Exception."), "Various exceptions");
		}
		void thenTimeReported() {
			confirm.that(run_time, equals(0.1), "TestUtilities should supply a standard run_time");
		}
		void test() {
			givenTestCase();
			whenCalled();
			thenExceptionReported();
			thenTimeReported();
		}
	} GPPUNIT_INSTANCE;

	class AssertExceptionTest: public VariousExceptions {
		AssertExceptionThrowingTestCase testcase6;
		virtual void givenTestCases() {
			add(testcase6);
		}
		virtual void thenExceptionReported() {
			confirm.isFalse(exceptionReported, "then_NO_ExceptionReported");
			confirm.that(collect, equals(""), "AssertException not collected");
		}
	} GPPUNIT_INSTANCE;

	class SetupThrowingTestCase: public IntThrowingTestCase {
		void setup() {
			throw std::runtime_error("rte");
		}
	};
	class ExceptionInSetup: public Utilities::TestCaseCaller, gppUnit::Notification {
		SetupThrowingTestCase testcase1;

		std::stringstream collect;

		void StartMethod(const gppUnit::MethodDescription& desc) {
			collect << desc.name() << '.';
		}

		void givenTestCase() {
			add(testcase1);
			givenNotification(this);
		}
		void thenOnlySetupAndTeardownRun() {
			confirm.that(collect, equals("setup.teardown."), "thenOnlySetupAndTeardownRun");
		}
		void test() {
			givenTestCase();
			whenCalled();
			thenOnlySetupAndTeardownRun();
		}
	} GPPUNIT_INSTANCE;

	class SetupFailTestCase: public IntThrowingTestCase {
		void setup() {
			gppUnit::TestResult result;
			result.message = "setup fails";
			reporter->Report(result);
		}
	};
	class FailInSetup: public Utilities::TestCaseCaller, gppUnit::Notification {
		SetupFailTestCase testcase1;

		std::stringstream collect;

		void StartMethod(const gppUnit::MethodDescription& desc) {
			collect << desc.name() << '.';
		}

		void givenTestCase() {
			add(testcase1);
			givenNotification(this);
		}
		void thenOnlySetupAndTeardownRun() {
			confirm.that(collect, equals("setup.teardown."), "thenOnlySetupAndTeardownRun");
		}
		void test() {
			givenTestCase();
			whenCalled();
			thenOnlySetupAndTeardownRun();
		}
	} GPPUNIT_INSTANCE;
}
