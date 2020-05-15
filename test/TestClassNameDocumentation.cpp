#include "src/ClassDocumentation.h"

#include "src/AutoRunner.h"

#include "TestUtilities.h"

namespace {
	class NopTestCase: public Utilities::MockTestCase {
		void test() {}
	};
}

class GCase1: public NopTestCase {
} gc1;
class GCase2: public NopTestCase {
} gc2;

namespace Alt1 {
	namespace Alt2 {
		class Case1: public NopTestCase {
		} ac1;
		class Case2: public NopTestCase {
		} ac2;
	}
}

namespace TestClassNameDocumentation {
	using gppUnit::equals;

	class Case1: public NopTestCase {
	} c1;
	class Case2: public NopTestCase {
	} c2;

	namespace NextLevel {
		class Case1: public NopTestCase {
		} nc1;
		class Case2: public NopTestCase {
		} nc2;
	}
	using namespace NextLevel;
	using namespace Alt1::Alt2;

	class MockDoc: public gppUnit::ClassDocumentation {
		//public:
		//    using gppUnit::ClassDocumentation::collect;
	};
	class TestDocumentation: public Auto::TestCase {
		gppUnit::CommandLineOptions options;
		gppUnit::AutoRunner runner{ options };
		MockDoc doc;

		gppUnit::TestCaseList cases;

		void setup() {
			runner << doc;
		}
	protected:
		void push_back(gppUnit::PrototypeTestCase& tc) {
			cases.push_back(&tc);
		}
		void whenRun() {
			runner.run(cases);
		}
		void thenDocumentationIs(const std::string& expected) {
			confirm.that(doc.getFormattedDocumentation(), equals(expected), __FUNCTION__);
		}
	};
	class GlobalTestCases: public TestDocumentation {
		void givenProject() {
			push_back(gc1);
			push_back(gc2);
		}
		void test() {
			givenProject();
			whenRun();
			thenDocumentationIs(
			    "GCase1\n"
			    "GCase2\n"
			);
		}
	} GPPUNIT_INSTANCE;
	class SingleNamespacedTestCases: public TestDocumentation {
		void givenProject() {
			push_back(c1);
			push_back(c2);
		}
		void test() {
			givenProject();
			whenRun();
			thenDocumentationIs(
			    "TestClassNameDocumentation\n"
			    " Case1\n"
			    " Case2\n"
			);
		}
	} GPPUNIT_INSTANCE;
	class NextLevelNamespacedTestCases: public TestDocumentation {
		void givenProject() {
			push_back(nc1);
			push_back(nc2);
		}
		void test() {
			givenProject();
			whenRun();
			thenDocumentationIs(
			    "TestClassNameDocumentation\n"
			    " NextLevel\n"
			    "  Case1\n"
			    "  Case2\n"
			);
		}
	} GPPUNIT_INSTANCE;
	class IncreasingNamespacedTestCases: public TestDocumentation {
		void givenProject() {
			push_back(gc1);
			push_back(gc2);
			push_back(c1);
			push_back(c2);
			push_back(nc1);
			push_back(nc2);
		}
		void test() {
			givenProject();
			whenRun();
			thenDocumentationIs(
			    "GCase1\n"
			    "GCase2\n"
			    "TestClassNameDocumentation\n"
			    " Case1\n"
			    " Case2\n"
			    " NextLevel\n"
			    "  Case1\n"
			    "  Case2\n"
			);
		}
	} GPPUNIT_INSTANCE;
	class DecreasingNamespacedTestCases: public TestDocumentation {
		void givenProject() {
			push_back(nc1);
			push_back(nc2);
			push_back(c1);
			push_back(c2);
			push_back(gc1);
			push_back(gc2);
		}
		void test() {
			givenProject();
			whenRun();
			thenDocumentationIs(
			    "TestClassNameDocumentation\n"
			    " NextLevel\n"
			    "  Case1\n"
			    "  Case2\n"
			    " Case1\n"
			    " Case2\n"
			    "GCase1\n"
			    "GCase2\n"
			);
		}
	} GPPUNIT_INSTANCE;
	class DecreasingNamespacedTestCasesWithAlt: public TestDocumentation {
		void givenProject() {
			push_back(nc1);
			push_back(nc2);
			push_back(ac1);
			push_back(ac2);
			push_back(c1);
			push_back(c2);
			push_back(gc1);
			push_back(gc2);
		}
		void test() {
			givenProject();
			whenRun();
			thenDocumentationIs(
			    "TestClassNameDocumentation\n"
			    " NextLevel\n"
			    "  Case1\n"
			    "  Case2\n"
			    "Alt1\n"
			    " Alt2\n"
			    "  Case1\n"
			    "  Case2\n"
			    "TestClassNameDocumentation\n"
			    " Case1\n"
			    " Case2\n"
			    "GCase1\n"
			    "GCase2\n"
			);
		}
	} GPPUNIT_INSTANCE;
}
