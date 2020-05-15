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
#include "Matchers/LogicalMatchers.h"

typedef std::vector<std::string> strvec;

namespace TestLogicalMatchers {
	using Utilities::MatcherHelper;
	using gppUnit::less_than;
	using gppUnit::greater_than;
	using gppUnit::equals;
	using gppUnit::any_of;
	using gppUnit::all_of;
	using gppUnit::tab;
	using gppUnit::endl;

	class TestLogicalMatchersHelper: public Auto::TestCase {
		template<typename MATCHER, typename ACTUAL>
		void That(bool booleanResult, const ACTUAL& actual, MATCHER m, const strvec& desc, bool hasActual) {
			gppUnit::MatcherResult result = m.match(actual);
			if(booleanResult) {
				confirm.isTrue(result.result, "result is true");
			} else {
				confirm.isFalse(result.result, "result is false");
			}

			const strvec& matcherDescription = result.strm.toVector();
			expect.that(matcherDescription.size(), equals(desc.size()), "Description has same number of lines");
			confirm.isTrue(desc == matcherDescription, "Whole Description should match");

			strvec::const_iterator test = matcherDescription.begin();
			for(strvec::const_iterator it = desc.begin(), end = desc.end(); it != end; ++it) {
				confirm.that(*test++, equals(*it), (*it).c_str());
			}
			confirm.that(result.hasActual, equals(hasActual), "has Actual");
		}

	protected:
		template<typename MATCHER, typename ACTUAL>
		void That(const ACTUAL& actual, MATCHER m, const strvec& desc) {
			That(true, actual, m, desc, true);
		}
		template<typename MATCHER, typename ACTUAL>
		void Mismatch(const ACTUAL& actual, MATCHER m, const strvec& desc) {
			That(false, actual, m, desc, true);
		}
	};
	namespace Any_Of {
		class TestIntegers: public TestLogicalMatchersHelper {
			void test() {
				int integer = 1;
				long longint = 2;
				signed char schar = -4;
				std::vector<MatcherHelper*> vec;

				gppUnit::TableFormatter tf;
				tf << "a match with one of:" << tab << "a value less than '1'" << endl;
				tf << "or" << tab << "'2'" << endl;
				That(longint, any_of(less_than(integer), equals(longint)), tf.toVector());

				tf.clear();
				tf << "a match with one of:" << tab << "a value less than '0'" << endl;
				tf << "or" << tab << "'1'" << endl;
				That(schar, any_of(less_than(vec.size()), equals(integer)), tf.toVector());

				tf.clear();
				tf << "a match with one of:" << tab << "a value greater than '0'" << endl;
				tf << "or" << tab << "'1'" << endl;
				Mismatch(schar, any_of(greater_than(vec.size()), equals(integer)), tf.toVector());

				tf.clear();
				tf << "not" << tab << "a match with one of:" << tab << "a value less than '0'" << endl;
				tf << tab << "or" << tab << "'1'" << endl;
				Mismatch(schar, !any_of(less_than(vec.size()), equals(integer)), tf.toVector());
			}
		} GPPUNIT_INSTANCE;

		class TestThreeTerms: public TestLogicalMatchersHelper {
			void test() {
				int integer = 1;
				long longint = 2;
				signed char schar = -4;
				std::vector<MatcherHelper*> vec;

				gppUnit::TableFormatter tf;
				tf << "a match with one of:" << tab << "a value less than '1'" << endl;
				tf << "or" << tab << "'2'" << endl;
				tf << "or" << tab << "a value less than '-4'" << endl;
				That(longint, any_of(less_than(integer), equals(longint), less_than(schar)), tf.toVector());
			}
		} GPPUNIT_INSTANCE;

		class TestFourTerms: public TestLogicalMatchersHelper {
			void test() {
				int integer = 1;
				long longint = 2;
				signed char schar = -4;
				std::vector<MatcherHelper*> vec;

				gppUnit::TableFormatter tf;
				tf << "a match with one of:" << tab << "a value less than '1'" << endl;
				tf << "or" << tab << "'2'" << endl;
				tf << "or" << tab << "a value less than '-4'" << endl;
				tf << "or" << tab << "a value less than '0'" << endl;
				That(longint, any_of(less_than(integer), equals(longint), less_than(schar), less_than(vec.size())), tf.toVector());

				tf.clear();
				tf << "not" << tab << "a match with one of:" << tab << "a value less than '1'" << endl;
				tf << tab << "or" << tab << "a value less than '2'" << endl;
				tf << tab << "or" << tab << "a value less than '-4'" << endl;
				tf << tab << "or" << tab << "a value less than '0'" << endl;
				That(longint, !any_of(less_than(integer), less_than(longint), less_than(schar), less_than(vec.size())), tf.toVector());
			}
		} GPPUNIT_INSTANCE;

	}
	namespace TestLogicalAndMatcher {
		class TestIntegers: public TestLogicalMatchersHelper {
			void test() {
				int integer = 1;
				long longint = 2;
				signed char schar = -4;
				std::vector<MatcherHelper*> vec;

				gppUnit::TableFormatter tf;
				tf << "a match with all of:" << tab << "a value less than '1'" << endl;
				tf << "and" << tab << "a value less than '2'" << endl;
				That(schar, all_of(less_than(integer), less_than(longint)), tf.toVector());

				tf.clear();
				tf << "a match with all of:" << tab << "'1'" << endl;
				tf << "and" << tab << "a value less than '2'" << endl;
				That(integer, all_of(equals(integer), less_than(longint)), tf.toVector());

				tf.clear();
				tf << "a match with all of:" << tab << "'1'" << endl;
				tf << "and" << tab << "a value less than '2'" << endl;
				Mismatch(schar, all_of(equals(integer), less_than(longint)), tf.toVector());

				tf.clear();
				tf << "not" << tab << "a match with all of:" << tab << "'1'" << endl;
				tf << tab << "and" << tab << "a value less than '2'" << endl;
				Mismatch(integer, !all_of(equals(integer), less_than(longint)), tf.toVector());
			}
		} GPPUNIT_INSTANCE;

		class TestThreeTerms: public TestLogicalMatchersHelper {
			void test() {
				int integer = 1;
				long longint = 2;
				signed char schar = -4;
				std::vector<MatcherHelper*> vec;

				gppUnit::TableFormatter tf;
				tf << "a match with all of:" << tab << "'1'" << endl;
				tf << "and" << tab << "a value less than '2'" << endl;
				tf << "and" << tab << "a value greater than '-4'" << endl;
				That(integer, all_of(equals(integer), less_than(longint), greater_than(schar)), tf.toVector());
			}
		} GPPUNIT_INSTANCE;

		class TestFourTerms: public TestLogicalMatchersHelper {
			void test() {
				int integer = 1;
				long longint = 2;
				signed char schar = -4;
				std::vector<MatcherHelper*> vec;

				gppUnit::TableFormatter tf;
				tf << "a match with all of:" << tab << "a value greater than '1'" << endl;
				tf << "and" << tab << "'2'" << endl;
				tf << "and" << tab << "a value greater than '-4'" << endl;
				tf << "and" << tab << "a value greater than '0'" << endl;
				That(longint, all_of(greater_than(integer), equals(longint), greater_than(schar), greater_than(vec.size())), tf.toVector());

				tf.clear();
				tf << "a match with all of:" << tab << "'1'" << endl;
				tf << "and" << tab << "a value less than '2'" << endl;
				tf << "and" << tab << "a value greater than '-4'" << endl;
				tf << "and" << tab << "'0'" << endl;
				Mismatch(integer, all_of(equals(integer), less_than(longint), greater_than(schar), equals(vec.size())), tf.toVector());

				tf.clear();
				tf << "not" << tab << "a match with all of:" << tab << "a value greater than '1'" << endl;
				tf << tab << "and" << tab << "'2'" << endl;
				tf << tab << "and" << tab << "a value greater than '-4'" << endl;
				tf << tab << "and" << tab << "a value greater than '0'" << endl;
				Mismatch(longint, !all_of(greater_than(integer), equals(longint), greater_than(schar), greater_than(vec.size())), tf.toVector());
			}
		} GPPUNIT_INSTANCE;
	}
}
