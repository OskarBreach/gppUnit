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
#include "SuccessLogger.h"

#include "ProjectDescription.h"


namespace gppUnit {
	using std::endl;

	void LoggerAlgorithm::LogToFile(const std::string& fileName, const ProjectDescription* project) {
		if(allowedToProceed(project)) {
			if(fileExists(fileName)) {
				openFileForAppend(fileName);
			} else {
				openFileForWriting(fileName);
				writeHeader(fileName, project);
			}
			writeLog(project);
			closeFile();
		}
	}

	bool FileLogger::fileExists(const std::string& fileName) {
		std::ifstream ifile;
		ifile.open(fileName.c_str(), std::ios::in);
		bool ret = ifile.is_open();
		ifile.close();
		return ret;
	}
	void FileLogger::openFileForAppend(const std::string& fileName) {
		file.open(fileName.c_str(), std::ios::out | std::ios::app);
	}
	void FileLogger::openFileForWriting(const std::string& fileName) {
		file.open(fileName.c_str(), std::ios::out);
	}
	void FileLogger::closeFile() {
		file.close();
	}

	bool SuccessLogger::allowedToProceed(const ProjectDescription* project) {
		return project->hasPassed();
	}
	void SuccessLogger::writeHeader(const std::string& fileName, const ProjectDescription* project) {
		file << "'table=1.1" << endl;
		file << "'" << fileName << " - Automatically created by gppUnit1.5" << endl;
		file << "unittests,name=" << project->name() << endl;
		file << "tests,units,date,time,runtime" << endl;
	}
	void SuccessLogger::writeLog(const ProjectDescription* project) {
		file << project->results() << ','
		     << project->classes() << ','
		     << getNow() << ','
		     << project->run_time()
		     << endl;
	}

	bool AllRunsLogger::allowedToProceed(const ProjectDescription*) {
		return true;
	}
	void AllRunsLogger::writeHeader(const std::string& fileName, const ProjectDescription* project) {
		file << "'table=1.1" << endl;
		file << "'" << fileName << " - Automatically created by gppUnit1.5" << endl;
		file << "unittests,name=" << project->name() << endl;
		file << "passed,tests,units,date,time,runtime" << endl;
	}
	void AllRunsLogger::writeLog(const ProjectDescription* project) {
		file << project->hasPassed() << ','
		     << project->results() << ','
		     << project->classes() << ','
		     << getNow() << ','
		     << project->run_time()
		     << endl;
	}
}