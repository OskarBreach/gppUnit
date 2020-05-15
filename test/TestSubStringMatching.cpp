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
#include "TestMatchersHelpers.h"
#include "Matchers/SubStringMatchers.h"



namespace TestSubStringMatching {
	using Utilities::MatcherHelper;
	using gppUnit::starts_with;
	using gppUnit::ends_with;
	using gppUnit::contains;

	class BeginsWith: public MatcherHelper {
		void test() {
			That("foo", starts_with("f"), "a string starting with 'f'\n");
			That("bar", !starts_with("f"), "not a string starting with 'f'\n");
			That("bar", !starts_with("doof"), "not a string starting with 'doof'\n");
		}
	} GPPUNIT_INSTANCE;
	class BeginsWithStdString: public MatcherHelper {
		void test() {
			std::string foo = "foo";
			That(foo, starts_with("f"), "a string starting with 'f'\n");
			That(foo, !starts_with("g"), "not a string starting with 'g'\n");
			That(foo, !starts_with("doof"), "not a string starting with 'doof'\n");
		}
	} GPPUNIT_INSTANCE;

	class EndsWith: public MatcherHelper {
		void test() {
			That("foo", ends_with("o"), "a string ending with 'o'\n");
			That("bar", !ends_with("o"), "not a string ending with 'o'\n");
			That("bar", !ends_with("doof"), "not a string ending with 'doof'\n");
		}
	} GPPUNIT_INSTANCE;
	class EndsWithStdString: public MatcherHelper {
		void test() {
			std::string foo = "foo";
			That(foo, ends_with("o"), "a string ending with 'o'\n");
			That(foo, !ends_with("p"), "not a string ending with 'p'\n");
			That(foo, !ends_with("doof"), "not a string ending with 'doof'\n");
		}
	} GPPUNIT_INSTANCE;

	class Contains: public MatcherHelper {
		void test() {
			That("bar", contains("b"), "a string that contains 'b'\n");
			That("bar", contains("a"), "a string that contains 'a'\n");
			That("bar", contains("r"), "a string that contains 'r'\n");
			That("bar", !contains("z"), "not a string that contains 'z'\n");
		}
	} GPPUNIT_INSTANCE;
	class ContainsStdString: public MatcherHelper {
		void test() {
			std::string bar = "bar";
			That(bar, contains("b"), "a string that contains 'b'\n");
			That(bar, contains("a"), "a string that contains 'a'\n");
			That(bar, contains("r"), "a string that contains 'r'\n");
			That(bar, !contains("z"), "not a string that contains 'z'\n");
		}
	} GPPUNIT_INSTANCE;

}
