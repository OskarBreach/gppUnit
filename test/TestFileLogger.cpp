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
#include "src/SuccessLogger.h"
#include "src/ProjectDescription.h"

#include "AutoRun.h"
#include "src/AssertException.h"

namespace TestFileLogger {
	using gppUnit::equals;

	class MockProject1: public gppUnit::ProjectDescription {
		virtual std::string name() const { return "MockProject1"; }
		virtual size_t classes() const { return 0; }
		virtual bool hasPassed() const { return false; }
		virtual size_t results() const { return 0; }
		virtual double run_time() const { return 0; }
	} mp1;
	class MockProject2: public MockProject1 {
		//virtual std::string name() const { return "MockProject2"; }
		virtual bool hasPassed() const { return true; }
	} mp2;

	class FileLoggerThatCanBeDeletedForCoverage: public gppUnit::FileLoggerInterface {
		void LogToFile(const std::string&, const gppUnit::ProjectDescription*) {}
	};
	class TestNotification: public Auto::TestCase, gppUnit::FileLoggerInterface {
		std::string fileName;
		const gppUnit::ProjectDescription* project;

		gppUnit::FileLoggerNotification* fileNotify;

		void LogToFile(const std::string& name, const gppUnit::ProjectDescription* project_) {
			fileName = name;
			project = project_;
		}

		void givenNotification() {
			fileName = "";
			project = 0;
			fileNotify = new gppUnit::FileLoggerNotification("foo", *this);
		}
		void whenStartAndEndProjectCalled() {
			gppUnit::Notification* notify = fileNotify;
			notify->StartProject(mp1);
			notify->EndProject();
		}
		void thenDetailsPassedToInterface() {
			confirm.that(fileName, equals("foo"), "filename should be foo");
			confirm.that((gppUnit::ProjectDescription*)project,
			             equals((gppUnit::ProjectDescription*)&mp1), "project is mp1");
		}
		void teardown() { delete fileNotify; }
		void test() {
			givenNotification();
			whenStartAndEndProjectCalled();
			thenDetailsPassedToInterface();
			FileLoggerThatCanBeDeletedForCoverage d;

			throw gppUnit::AssertException();
		}
	} GPPUNIT_INSTANCE;

	class TestMethodBase : public Auto::TestCase {
		bool testHasRun;
		void setup() {
			testHasRun = false;
		}
		void test() {
			testHasRun = true;
		}
	protected:
		void givenTestHasNotRun() {
			testHasRun = false;
		}
		void thenTestMethodHasNotRun() {
			confirm.isFalse(testHasRun, "test method should not have run");
		}
		void thenTestMethodHasRun() {
			confirm.isTrue(testHasRun, "test method should have run");
		}
	} GPPUNIT_INSTANCE;

	class SetupThrows : public TestMethodBase {
		void setup() {
			givenTestHasNotRun();
			throw gppUnit::AssertException();
		}
		void teardown() {
			thenTestMethodHasNotRun();
		}
	} GPPUNIT_INSTANCE;
	class SetupDoesNotThrow : public TestMethodBase {
		void teardown() {
			thenTestMethodHasRun();
		}
	} GPPUNIT_INSTANCE;

	class SuccessLoggerTestBase: public Auto::TestCase, gppUnit::LoggerAlgorithm {

		mutable std::stringstream collect;
		gppUnit::FileLoggerNotification* fileNotify;
		bool exists;

		bool allowedToProceed(const gppUnit::ProjectDescription* project) const {
			return project->hasPassed();
		}
		virtual bool fileExists(const std::string& fileName) const {
			collect << "e." << fileName << '.';
			return exists;
		}
		virtual void openFileForAppend(const std::string& fileName) {
			collect << "a." << fileName << '.';
		}
		virtual void openFileForWriting(const std::string& fileName) {
			collect << "o." << fileName << '.';
		}
		virtual void writeHeader(const std::string& fileName, const gppUnit::ProjectDescription* /*project*/) {
			collect << "h." << fileName << '.';
		}
		virtual std::ostream& getFile() { return collect; }

		void writeLog(const gppUnit::ProjectDescription* /*project*/) { collect << "w."; }
		void closeFile() { collect << "c."; }

		void teardown() { delete fileNotify; }
	protected:
		void givenNotification() {
			fileNotify = new gppUnit::FileLoggerNotification("foo", *this);
			exists = true;
		}
		void whenStartAndEndProjectCalled(const gppUnit::ProjectDescription& desc, bool exists_ = true) {
			exists = exists_;
			gppUnit::Notification* notify = fileNotify;
			notify->StartProject(desc);
			notify->EndProject();
		}
		void thenNoActionTaken() {
			confirm.that(collect.str(), equals(""), "No action");
		}
		void thenActionTakenIs(const char* action) {
			confirm.that(collect.str(), equals(action), "Expected action");
		}
	};
	class SuccessLoggerTakesNoAction: public SuccessLoggerTestBase {

		void test() {
			givenNotification();
			whenStartAndEndProjectCalled(mp1);
			thenNoActionTaken();
		}
	} GPPUNIT_INSTANCE;

	class SuccessLoggerAppendsToBase: public SuccessLoggerTestBase {
		void test() {
			givenNotification();
			whenStartAndEndProjectCalled(mp2);
			thenActionTakenIs("e.foo.a.foo.w.c.");
		}
	} GPPUNIT_INSTANCE;
	class SuccessLoggerWritesHeader: public SuccessLoggerTestBase {
		void test() {
			givenNotification();
			whenStartAndEndProjectCalled(mp2, false);
			thenActionTakenIs("e.foo.o.foo.h.foo.w.c.");
		}
	} GPPUNIT_INSTANCE;

	class TestLoggerImplementations: public Auto::TestCase, public virtual gppUnit::LoggerAlgorithm {
		mutable std::stringstream collect;
		virtual std::ostream& getFile() { return collect; }
		// deliberate override of getNow()
		const char* getNow() { return "date,time"; }
		// nop implementations!!
		bool fileExists(const std::string&) const { return false; }
		void openFileForAppend(const std::string&) {}
		void openFileForWriting(const std::string&) {}
		void closeFile() {}
		void setup() {
			//COVERAGE:
			std::string name;
			fileExists(name);
			openFileForAppend(name);
			openFileForWriting(name);
			closeFile();
		}
	protected:
		void whenHeaderWritten(const gppUnit::ProjectDescription& project) {
			writeHeader("filename", &project);
		}
		void whenLogWritten(const gppUnit::ProjectDescription& project) {
			writeLog(&project);
		}
		void thenAllowedToProceed(const gppUnit::ProjectDescription& project, bool expected) {
			confirm.that(allowedToProceed(&project), equals(expected), "Should be allowed to proceed");
		}
		void thenOutputIs(const std::string& expected, const char* message) {
			confirm.that(collect.str(), equals(expected), message);
		}
	};
	class TestSuccessLoggerImplementationAllowedToProceed: public TestLoggerImplementations, gppUnit::SuccessLoggerImplementation {
		void test() {
			thenAllowedToProceed(mp1, false);
			thenAllowedToProceed(mp2, true);
		}
	} GPPUNIT_INSTANCE;
	class TestSuccessLoggerImplementationWriteHeader: public TestLoggerImplementations, gppUnit::SuccessLoggerImplementation {
		void test() {
			whenHeaderWritten(mp1);
			thenOutputIs(
			    "'table=1.1\n"
			    "'filename - Automatically created by gppUnit1.5\n"
			    "unittests,name=MockProject1\n"
			    "tests,units,date,time,runtime\n"
			    , "Success Header");
		}
	} GPPUNIT_INSTANCE;
	class TestSuccessLoggerImplementationWriteLog: public TestLoggerImplementations, gppUnit::SuccessLoggerImplementation {
		void test() {
			whenLogWritten(mp1);
			thenOutputIs(
			    "0,0,date,time,0\n"
			    , "Success Header");
		}
	} GPPUNIT_INSTANCE;

	class TestAllRunsLoggerImplementation: public TestLoggerImplementations, gppUnit::AllRunsLoggerImplementation {
		void test() {
			thenAllowedToProceed(mp1, true);
			thenAllowedToProceed(mp2, true);
		}
	} GPPUNIT_INSTANCE;
	class TestAllRunsLoggerImplementationWriteHeader: public TestLoggerImplementations, gppUnit::AllRunsLoggerImplementation {
		void test() {
			whenHeaderWritten(mp1);
			thenOutputIs(
			    "'table=1.1\n"
			    "'filename - Automatically created by gppUnit1.5\n"
			    "unittests,name=MockProject1\n"
			    "passed,tests,units,date,time,runtime\n"
			    , "Success Header");
		}
	} GPPUNIT_INSTANCE;
	class TestAllRunsLoggerImplementationWriteLog: public TestLoggerImplementations, gppUnit::AllRunsLoggerImplementation {
		void test() {
			whenLogWritten(mp1);
			thenOutputIs(
			    "0,0,0,date,time,0\n"
			    , "Success Header");
		}
	} GPPUNIT_INSTANCE;

	class MockFileLoggerForCoverage: public Auto::TestCase, public virtual gppUnit::LoggerAlgorithm {
		// nop implementations!!
		bool allowedToProceed(const gppUnit::ProjectDescription*) const { return false; }
		void writeHeader(const std::string&, const gppUnit::ProjectDescription*) {}
		void writeLog(const gppUnit::ProjectDescription*) {}
	protected:
		std::ios_base::openmode mode;
		void givenLogger() { mode = std::ios_base::openmode(0); }
		void whenOpenForAppend() {
			openFileForAppend("");
		}
		void whenOpenForWriting() {
			openFileForWriting("");
		}
		void thenMode(std::ios_base::openmode expected, const char* message) {
			confirm.that(mode, equals(expected), message);
		}
	};

	class TestFileLoggerForCoverage: public MockFileLoggerForCoverage, gppUnit::FileLogger {
		void openFile(const std::string&, std::ios_base::openmode mode_) {
			mode = mode_;
		}
		void test() {
			givenLogger();
			whenOpenForAppend();
			thenMode(std::ios::out | std::ios::app, "OpenForAppend");
			givenLogger();
			whenOpenForWriting();
			thenMode(std::ios::out, "OpenForWriting");
		}
	} GPPUNIT_INSTANCE;
}
