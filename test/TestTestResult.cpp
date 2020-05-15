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
#include "src/TestResult.h"

#include "AutoRun.h"

namespace TestTestResult {
	using gppUnit::equals;

	class Defaults: public Auto::TestCase {
		gppUnit::TestResult result;
		void givenTestResult() {
			result = gppUnit::TestResult();
		}
		void thenResultIsFalse() {
			confirm.isFalse(result.result, "Result is false by default");
		}
		void thenMessageIsEmpty() {
			confirm.that(result.message, equals(""), "Message is empty");
			confirm.isTrue(result.message.empty(), "Message is empty");
		}
		void thenDescriptionIsEmpty() {
			confirm.isTrue(result.description.empty(), "Description is empty");
		}
		void test() {
			givenTestResult();
			thenResultIsFalse();
			thenMessageIsEmpty();
			thenDescriptionIsEmpty();
		}
	} GPPUNIT_INSTANCE;
}
