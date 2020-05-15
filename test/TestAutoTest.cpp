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
#include "src/AutoTest.h"

#include "TestUtilities.h"

namespace TestAutoTest {
	using gppUnit::equals;

	// Warning: NEVER make an instance of MockTest on the stack or globally - always make an instance on the HEAP!!!
	class MockTest: public Utilities::MockTestCase, public gppUnit::AutoTest {
		void test() {}
	};
	class Test: public Auto::TestCase {
		gppUnit::TestCaseList mockList;
		gppUnit::TestCase* mock;
		MockTest* actualMock;
		void setup() {
			// Give the global AutoList our list
			gppUnit::globalAutoList().autoTests(mockList);
			// Make a new test that registers itself
			mock = actualMock = new MockTest();
		}
		void test() {
			expect.that(mock, !gppUnit::is_null(), "Setup produced a test instance");
			expect.that(mockList.size(), equals(1), "A test is registered with mockList");
			confirm.that(gppUnit::autoTests().size(), equals(1), "A test is registered with mockList");
			confirm.that(mockList.front(), equals(mock), "mock is at front");

			//COVERAGE: Actually need to run the test method
			mock->test();
		}
		void teardown() {
			gppUnit::globalAutoList().autoTests(Utilities::dummyTestCaseList);
			delete actualMock;
		}
	} GPPUNIT_INSTANCE;
}
