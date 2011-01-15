#ifndef TESTRESULT_H_4B24D178_9638_4573_B221_C79E64AAC3E2
#define TESTRESULT_H_4B24D178_9638_4573_B221_C79E64AAC3E2

#include <string>
#include <vector>

namespace gppUnit{
	struct TestResult{
		bool result;
		std::string message;
		std::vector<std::string> description;
		TestResult():result(false),message(),description(){}
	};
}

#endif // TESTRESULT_H_4B24D178_9638_4573_B221_C79E64AAC3E2
