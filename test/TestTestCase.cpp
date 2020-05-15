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

#include "TestUtilities.h"

#include <sstream>

namespace TestTestCase {
	using gppUnit::equals;

	class MockTestCase: public Utilities::MockTestCase {
		std::stringstream collect;
		void setup() { collect << gppUnit::setupMethodName() << '.'; }
		void test() { collect << gppUnit::testMethodName() << '.'; }
		void teardown() { collect << gppUnit::teardownMethodName() << '.'; }
	public:
		std::string result() {return collect.str();}
	};

	std::string setuptestteardownString() {
		return std::string(gppUnit::setupMethodName()) + '.' +
		       gppUnit::testMethodName() + '.' +
		       gppUnit::teardownMethodName() + '.';
	}

	class TestMethodNames: public Auto::TestCase {
		void test() {

			confirm.that(gppUnit::setupMethodName(), equals("setup"), "Should be setup");

			confirm.that(gppUnit::testMethodName(), equals("test"), "Should be test");

			confirm.that(gppUnit::teardownMethodName(), equals("teardown"), "Should be teardown");
		}
	} GPPUNIT_INSTANCE;

	class AllThreeMethodsCalled: public Utilities::TestCaseCaller {
		MockTestCase testcase;

		void givenMockTestCase() {
			add(testcase);
		}
		void thenAllThreeMethodsCalled() {
			confirm.that(testcase.result(), equals(setuptestteardownString()), "Should have called three methods");
		}
		void test() {
			givenMockTestCase();
			whenCalled();
			thenAllThreeMethodsCalled();
		}
	} GPPUNIT_INSTANCE;

	class TwoTestCasesCalled: public Utilities::TestCaseCaller {
		MockTestCase testcase1;
		MockTestCase testcase2;
		void givenTwoTestCases() {
			add(testcase1);
			add(testcase2);
		}
		void thenSixMethodsCalled() {
			std::string expected = setuptestteardownString() + setuptestteardownString();
			std::string actual = testcase1.result() + testcase2.result();
			confirm.that(actual, equals(expected), "Should have called six methods");
		}
		void test() {
			givenTwoTestCases();
			whenCalled();
			thenSixMethodsCalled();
		}
	} GPPUNIT_INSTANCE;
}
