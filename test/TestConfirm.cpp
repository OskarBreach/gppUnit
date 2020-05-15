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
#include "src/Confirmation.h"

#include "src/ReportResult.h"
#include "src/AssertException.h"
#include "src/TestResult.h"
#include "Matchers/SubStringMatchers.h"

#include "AutoRun.h"

typedef std::vector<std::string> strvec;

namespace TestAsserts {
	using gppUnit::equals;

	class Base: public Auto::TestCase, gppUnit::ReportResult {
		gppUnit::TestResult testResult;
		gppUnit::CommandLineOptions options;
		void Report(const gppUnit::TestResult& result) override {
			testResult = result;
		}
		std::string className() override { return ""; }
		const gppUnit::CommandLineOptions& getOptions() override { return options; }
		const strvec& description() { return testResult.description; }
		size_t descriptionSize() { return testResult.description.size(); }
		void setup() {
			//COVERAGE:
			className();
			getOptions();
		}
	protected:
		void setReportOnSetter(gppUnit::ResultSetter& setter) {
			setter.setReport(this);
		}
		void thenDescriptionIsEmpty() {
			confirm.that(descriptionSize(), equals(0), "Description is empty");
		}
		void thenDescriptionIs(const std::string& desc) {
			expect.that(descriptionSize(), equals(1), "Description has one line");
			confirm.that(description().front(), equals(desc), desc.c_str());
		}
		void thenDescriptionIs(const strvec& desc) {
			expect.that(description().size(), equals(desc.size()), "Description has same number of lines");
			confirm.isTrue(desc == description(), "Whole Description should match");
			strvec::const_iterator test = description().begin();
			for(strvec::const_iterator it = desc.begin(), end = desc.end(); it != end; ++it) {
				confirm.that(*test++, equals(*it), (*it).c_str());
			}
		}
		void thenResultIsFail() {
			confirm.isFalse(testResult.result, "thenResultIsFail");
			confirm.that(testResult.message, equals("fail"), "message is fail");
			//thenDescriptionIsEmpty();
		}
		void thenResultIsPass() {
			confirm.isTrue(testResult.result, "thenResultIsPass");
			confirm.that(testResult.message, equals("pass"), "message is pass");
			thenDescriptionIsEmpty();
		}
		void thenResultIsFalse(const char* message) {
			confirm.isFalse(testResult.result, "thenResultIsFalse");
			confirm.that(testResult.message, equals(message), message);
			//thenDescriptionIsEmpty();
		}
		void thenResultIsTrue(const char* message) {
			confirm.isTrue(testResult.result, "thenResultIsTrue");
			confirm.that(testResult.message, equals(message), message);
			//thenDescriptionIsEmpty();
		}
	};
	class ConfirmBase: public Base {
	protected:
		gppUnit::Confirm conf;
		void givenConfirm() {
			setReportOnSetter(conf);
		}
	};
	class ConfirmPassAndFail: public ConfirmBase {
		void whenFailCalled() {
			conf.fail("fail");
		}
		void whenPassCalled() {
			conf.pass("pass");
		}
		void test() {
			givenConfirm();
			whenFailCalled();
			thenResultIsFail();
			whenPassCalled();
			thenResultIsPass();
		}
	} GPPUNIT_INSTANCE;
	class ConfirmIsTrue: public ConfirmBase {
		void whenIsTrueCalled(bool value) {
			conf.isTrue(value);
		}
		void test() {
			givenConfirm();
			whenIsTrueCalled(false);
			thenResultIsFalse("Should be True");
			whenIsTrueCalled(true);
			thenResultIsTrue("Should be True");
		}
	} GPPUNIT_INSTANCE;
	class ConfirmIsFalse: public ConfirmBase {
		void whenIsFalseCalled(bool value) {
			conf.isFalse(value);
		}
		void test() {
			givenConfirm();
			whenIsFalseCalled(true);
			thenResultIsFalse("Should be False");
			whenIsFalseCalled(false);
			thenResultIsTrue("Should be False");
		}
	} GPPUNIT_INSTANCE;

	class CatchExceptionsFromExpect: public Base {
		gppUnit::Expect exp;
		bool caughtException;
	protected:
		void givenExpect() {
			setReportOnSetter(exp);
			caughtException = false;
		}
		void whenFailCalled() {
			try {
				exp.fail("fail");
			} catch(gppUnit::AssertException&) {
				caughtException = true;
			}
		}
		void whenThatCalled() {
			try {
				exp.that(this, gppUnit::is_null(), "fail");
			} catch(gppUnit::AssertException&) {
				caughtException = true;
			}
		}
		void thenExceptionCaught() {
			confirm.isTrue(caughtException, "thenExceptionCaught");
		}
		void whenPassCalled() {
			exp.pass("pass");
		}
	};
	class CatchExceptionsFromExpectFail: public CatchExceptionsFromExpect {
		void test() {
			givenExpect();
			whenFailCalled();
			thenResultIsFail();
			thenExceptionCaught();
			whenPassCalled();
			thenResultIsPass();
		}
	} GPPUNIT_INSTANCE;
	class CatchExceptionsFromExpectThat: public CatchExceptionsFromExpect {
		void test() {
			givenExpect();
			whenThatCalled();
			thenResultIsFail();
			thenExceptionCaught();
		}
	} GPPUNIT_INSTANCE;
}
namespace ConfirmThatDescription {
	using gppUnit::but_got;
	using gppUnit::and_got;
	using gppUnit::equals;

	class ConfirmThatDescriptionElements: public Auto::TestCase {
		void test() {
			confirm.that(but_got, equals("but got"));
			confirm.that(and_got, equals("and got"));
		}
	} GPPUNIT_INSTANCE;
}
namespace ConfirmThat {
	using TestAsserts::Base;
	using gppUnit::TableFormatter;
	using gppUnit::tab;
	using gppUnit::endl;
	using gppUnit::but_got;
	using gppUnit::and_got;

	class ConfirmThatResult: public TestAsserts::ConfirmBase {
		void test() {
			givenConfirm();
			conf.that(0, gppUnit::equal_to(0));
			thenResultIsTrue("Should match");
		}
	} GPPUNIT_INSTANCE;
	class ConfirmThatPassDescription: public TestAsserts::ConfirmBase {
		void test() {
			givenConfirm();
			conf.that(0, gppUnit::equal_to(0));

			gppUnit::TableFormatter f;
			f << "Expected" << tab << "'0'" << endl;
			f << and_got << tab << "'0'" << endl;
			thenDescriptionIs(f.toVector());
		}
	} GPPUNIT_INSTANCE;

	class ConfirmThatFailDescription: public TestAsserts::ConfirmBase {
		void test() {
			givenConfirm();
			conf.that(1, gppUnit::equal_to(0));
			thenResultIsFalse("Should match");

			gppUnit::TableFormatter f;
			f << "Expected" << tab << "'0'" << endl;
			f << but_got << tab << "'1'" << endl;
			thenDescriptionIs(f.toVector());
		}
	} GPPUNIT_INSTANCE;
	class ConfirmNotThatFailDescription: public TestAsserts::ConfirmBase {
		void test() {
			givenConfirm();
			conf.that(0, is_not(gppUnit::equal_to(0)));
			thenResultIsFalse("Should match");

			gppUnit::TableFormatter f;
			f << "Expected" << tab << "not" << tab << "'0'" << endl;
			f << but_got << tab << "'0'" << endl;
			thenDescriptionIs(f.toVector());

		}
	} GPPUNIT_INSTANCE;
	class ConfirmThatFailDescriptionIntChar: public TestAsserts::ConfirmBase {
		void test() {
			givenConfirm();
			long longint = 2;
			signed char schar = -4;

			conf.that(schar, gppUnit::equal_to(longint));
			thenResultIsFalse("Should match");

			gppUnit::TableFormatter f;
			f << "Expected" << tab << "'2'" << endl;
			f << but_got << tab << "'-4'" << endl;
			thenDescriptionIs(f.toVector());
		}
	} GPPUNIT_INSTANCE;
	class ConfirmThatFailDescriptionCharsChars: public TestAsserts::ConfirmBase {
		void test() {
			givenConfirm();
			const char* actual = "1";
			const char* expected = "2";

			conf.that(actual, gppUnit::equal_to(expected));
			thenResultIsFalse("Should match");

			gppUnit::TableFormatter f;
			f << "Expected" << tab << "string '2'" << endl;
			f << gppUnit::but_got << tab << "actual" << tab << "'1'" << endl;
			thenDescriptionIs(f.toVector());
		}
	} GPPUNIT_INSTANCE;
	class ConfirmThatFailDescriptionCharsCharArray: public TestAsserts::ConfirmBase {
		void test() {
			givenConfirm();
			const char* actual = "1";
			const char expected[] = "2";

			conf.that(actual, gppUnit::equal_to(expected));
			thenResultIsFalse("Should match");

			gppUnit::TableFormatter f;
			f << "Expected" << tab << "string '2'" << endl;
			f << gppUnit::but_got << tab << "actual" << tab << "'1'" << endl;
			thenDescriptionIs(f.toVector());
		}
	} GPPUNIT_INSTANCE;
	class ConfirmThatFailDescriptionVectorContains: public TestAsserts::ConfirmBase {
		void test() {
			givenConfirm();
			std::vector<int> v;

			conf.that(v, gppUnit::contains(1));
			thenResultIsFalse("Should match");

			gppUnit::TableFormatter f;
			f << "Expected" << tab << "a container that contains '1'" << endl;
			f << but_got << tab << "an empty container" << endl;
			thenDescriptionIs(f.toVector());
		}
	} GPPUNIT_INSTANCE;
}
namespace ConfirmationCoverage {
	class InstantiateConfirmationBase: public Auto::TestCase {
		void test() {
			gppUnit::ConfirmationBase cb;
		}
	} GPPUNIT_INSTANCE;
}

