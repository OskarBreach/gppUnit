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
#include "src/StreamNotification.h"
#include "src/ProjectDescription.h"
#include "src/ClassDescription.h"
#include "src/MethodDescription.h"
#include "src/TestResult.h"

#include "AutoRun.h"

#include <sstream>

namespace TestPrintFormatter {
	using gppUnit::equals;

	class PrintFormatterHelper: public Auto::TestCase {
		gppUnit::PrintFormatter* format;
		std::string result;
		std::string update;
	protected:
		void givenFormatter(size_t size) {
			format = new gppUnit::PrintFormatter(size);
		}
		void whenCentering(const std::string& title) {
			result = format->centreInAsterisks(title);
		}
		void whenUpdatedWith(size_t max, size_t current) {
			update = format->updateRunningAsterisks(max, current);
		}
		void thenResultIs(const std::string& expected) {
			confirm.that(result, equals(expected), "Result is centred");
		}
		void thenUpdateIs(const std::string& expected) {
			confirm.that(update, equals(expected), "Update is correct");
		}
		void teardown() { delete format; }
	};

	class SingleCharTitle: public PrintFormatterHelper {
		void test() {
			givenFormatter(5);
			whenCentering("x");
			thenResultIs("* x *");
		}
	} GPPUNIT_INSTANCE;
	class SingleCharTitleMultipleAsterisks: public PrintFormatterHelper {
		void test() {
			givenFormatter(9);
			whenCentering("x");
			thenResultIs("*** x ***");
		}
	} GPPUNIT_INSTANCE;
	class SingleCharTitleNonSymmetrical: public PrintFormatterHelper {
		void test() {
			givenFormatter(8);
			whenCentering("x");
			thenResultIs("** x ***");
		}
	} GPPUNIT_INSTANCE;

	class TitleLongerThanField: public PrintFormatterHelper {
		void test() {
			givenFormatter(5);
			whenCentering("TitleLongerThanField");
			thenResultIs("* T *");
		}
	} GPPUNIT_INSTANCE;

	class LongTitleLongField: public PrintFormatterHelper {
		void test() {
			givenFormatter(50);
			whenCentering("LongTitleLongField");
			thenResultIs("*************** LongTitleLongField ***************");
		}
	} GPPUNIT_INSTANCE;
	class LongTitleLongFieldNonSymmetrical: public PrintFormatterHelper {
		void test() {
			givenFormatter(50);
			whenCentering("LongTitleLongFieldNonSymmetrica");
			thenResultIs("******** LongTitleLongFieldNonSymmetrica *********");
		}
	} GPPUNIT_INSTANCE;

	class RunningAsterisks: public PrintFormatterHelper {
		void test() {
			givenFormatter(5);
			whenUpdatedWith(5, 0);
			thenUpdateIs("");

			whenUpdatedWith(5, 1);
			thenUpdateIs("*");

			whenUpdatedWith(5, 1);
			thenUpdateIs("");

			whenUpdatedWith(5, 3);
			thenUpdateIs("**");

			whenUpdatedWith(5, 5);
			thenUpdateIs("**");

			whenUpdatedWith(5, 5);
			thenUpdateIs("");

			whenUpdatedWith(5, 6);
			thenUpdateIs("");
		}
	} GPPUNIT_INSTANCE;
}
namespace TestStreamNotification {
	using gppUnit::equals;

	class MockProject1: public gppUnit::ProjectDescription {
		virtual std::string name() const { return "MockProject1"; }
		virtual size_t classes() const { return 5; }
		virtual bool hasPassed() const { return 0; }
		virtual size_t results() const { return 0; }
		virtual double run_time() const { return 0; }
	} mp1;
	class MockProject2: public MockProject1 {
		virtual std::string name() const { return "MockProject2"; }
		virtual size_t results() const { return 1; }
		virtual double run_time() const { return 1; }
	} mp2;
	class MockClass1: public gppUnit::ClassDescription {
		virtual std::string name() const { return "MockClass1"; }
		virtual size_t methods() const { return 0; }
		virtual size_t results() const { return 0; }
		virtual double run_time() const { return 0; }
	} mc1;
	class MockMethod1: public gppUnit::MethodDescription {
		virtual std::string name() const { return "MockMethod1"; }
		virtual size_t results() const { return 0; }
		virtual double run_time() const { return 0; }
	} mm1;
	class MockMethod2: public MockMethod1 {
		virtual std::string name() const { return "MockMethod2"; }
	} mm2;
	class StreamNotificationHelper: public Auto::TestCase {
		std::stringstream out;
		gppUnit::StreamNotification* stream;
		gppUnit::Notification* notify;
	protected:
		void givenStreamNotification() {
			notify = stream = new gppUnit::StreamNotification(out);
		}
		void givenStartMethod() {
			notify->StartProject(mp1);
			notify->StartClass(mc1);
			notify->StartMethod(mm1);
			out.str("");
		}
		void whenStartProjectCalled() {
			notify->StartProject(mp1);
		}
		void whenEndProjectCalled() {
			notify->EndProject();
		}
		void whenStartClassCalled() {
			whenStartProjectCalled();
			out.str("");
			notify->StartClass(mc1);
		}
		void whenStartClassCalledAgain() {
			out.str("");
			notify->StartClass(mc1);
		}
		void whenNextMethodCalled() {
			notify->StartMethod(mm2);
		}
		void whenResultPass() {
			notify->StartProject(mp1);
			notify->StartClass(mc1);
			out.str("");
			notify->StartMethod(mm1);
			gppUnit::TestResult result;
			result.result = true;
			notify->Result(result);
		}
		void whenSecondProjectRun() {
			notify->StartProject(mp2);
		}
		void whenResultFailed(const char* message = "", const std::string& description = "") {
			gppUnit::TestResult result;
			result.message = message;
			if(description.size() > 0) {
				result.description.push_back(description);
			}
			notify->Result(result);
		}
		void whenException(const std::string& what) {
			notify->Exception(what);
		}

		void thenProjectTitleAndClassesPrinted() {
			const char* expected =
			    "****************** MockProject1 ******************\n"
			    "5 classes to run.\n";
			confirm.that(out, equals(expected));
		}
		void thenOneHundredPercent() {
			const char* expected =
			    "\n"
			    "100% tests passed!\n"
			    "**************************************************\n";
			confirm.that(out, equals(expected));
		}
		void thenRunTimeIsOne() {
			const char* expected =
			    "****************** MockProject2 ******************\n"
			    "5 classes to run.\n"
			    "\n"
			    "100% tests passed!\n"
			    "1 Assert\n"
			    "run time: 1\n"
			    "**************************************************\n";
			confirm.that(out, equals(expected));
		}
		void thenNumberOfAssertsPrinted() {
			const char* expected =
			    "****************** MockProject2 ******************\n"
			    "5 classes to run.\n"
			    "\n"
			    "100% tests passed!\n"
			    "1 Assert\n"
			    "run time: 1\n"
			    "**************************************************\n";
			confirm.that(out, equals(expected));
		}

		void thenAsteriskPrinted() {
			const char* expected = "**********";
			confirm.that(out, equals(expected));
		}
		void thenNothingPrinted() {
			confirm.that(out, equals(""));
		}
		void thenExceptionPrinted() {
			const char* expected =
			    "\n"
			    " In Class: MockClass1\n"
			    "  In Method: MockMethod1\n"
			    "   what\n";
			confirm.that(out, equals(expected));
		}
		void thenFailurePrinted() {
			const char* expected =
			    "\n"
			    " In Class: MockClass1\n"
			    "  In Method: MockMethod1\n"
			    "   Failure\n";
			confirm.that(out, equals(expected));
		}
		void thenFullResultPrinted() {
			const char* expected =
			    "\n"
			    " In Class: MockClass1\n"
			    "  In Method: MockMethod1\n"
			    "   Message: message\n";
			confirm.that(out, equals(expected));
		}
		void thenMessageAndDescriptionPrinted() {
			const char* expected =
			    "\n"
			    " In Class: MockClass1\n"
			    "  In Method: MockMethod1\n"
			    "   Message: message\n"
			    "    Description\n"
			    ;
			confirm.that(out, equals(expected));
		}
		void thenTwoMessagesPrinted() {
			const char* expected =
			    "\n"
			    " In Class: MockClass1\n"
			    "  In Method: MockMethod1\n"
			    "   Message: message1\n"
			    "   Message: message2\n";
			confirm.that(out.str(), equals(expected));
		}
		void thenTwoMethodsAndMessagesPrinted() {
			const char* expected =
			    "\n"
			    " In Class: MockClass1\n"
			    "  In Method: MockMethod1\n"
			    "   Message: message1\n"
			    "  In Method: MockMethod2\n"
			    "   Message: message2\n";
			confirm.that(out, equals(expected));
		}
		void teardown() { delete stream; }
	};

	class ProjectStarted: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			whenStartProjectCalled();
			thenProjectTitleAndClassesPrinted();
		}
	} GPPUNIT_INSTANCE;
	class ClassStarted: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			whenStartClassCalled();
			thenAsteriskPrinted();
			whenStartClassCalledAgain();
			thenAsteriskPrinted();
		}
	} GPPUNIT_INSTANCE;
	class ResultPassed: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			whenResultPass();
			thenNothingPrinted();
			whenEndProjectCalled();
			thenOneHundredPercent();
		}
	} GPPUNIT_INSTANCE;
	class ResultPassedAndRunTime: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			whenSecondProjectRun();
			whenEndProjectCalled();
			thenRunTimeIsOne();
		}
	} GPPUNIT_INSTANCE;
	class ResultPassedAndNumberOfAsserts: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			whenSecondProjectRun();
			whenEndProjectCalled();
			thenNumberOfAssertsPrinted();
		}
	} GPPUNIT_INSTANCE;

	class ResultIsFailure: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			givenStartMethod();
			whenResultFailed();
			thenFailurePrinted();
		}
	} GPPUNIT_INSTANCE;
	class ResultIsFullMessage: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			givenStartMethod();
			whenResultFailed("message");
			thenFullResultPrinted();
		}
	} GPPUNIT_INSTANCE;

	class ResultIsMessageAndDescription: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			givenStartMethod();
			whenResultFailed("message", "Description");
			thenMessageAndDescriptionPrinted();
		}
	} GPPUNIT_INSTANCE;

	class TwoFailures: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			givenStartMethod();
			whenResultFailed("message1");
			whenResultFailed("message2");
			thenTwoMessagesPrinted();
		}
	} GPPUNIT_INSTANCE;
	class TwoFailuresInDifferentMethods: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			givenStartMethod();
			whenResultFailed("message1");
			whenNextMethodCalled();
			whenResultFailed("message2");
			thenTwoMethodsAndMessagesPrinted();
		}
	} GPPUNIT_INSTANCE;
	class OneException: StreamNotificationHelper {
		void test() {
			givenStreamNotification();
			givenStartMethod();
			whenException("what");
			thenExceptionPrinted();
		}
	} GPPUNIT_INSTANCE;
}
