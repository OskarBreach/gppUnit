/*
Copyright (c) 2011-2019 Andrew Wall

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
#include "TextFileApprover.h"

#include <fstream>
#include <vector>
#include <numeric>
#include <sstream>

namespace gppUnit {
	bool TextFileApprover::verify() {
		if(actualAndApprovedMatch()) {
			return true;
		}
		writeReceived();
		reportToUser();
		return actualAndApprovedMatch();
	}
	bool TextFileApprover::actualAndApprovedMatch() {
		approvedContents = getApprovedContents();
		if(actual == approvedContents) {
			removeReceived();
			return true;
		}
		return false;
	}

	std::string TextFileApprover::getApprovedContents() { return getFileContents(namer.getApprovedFilename()); }
	void TextFileApprover::writeReceived() {
		makeFileWithContents(namer.getReceivedFilename(), actual);
	}
	void TextFileApprover::removeReceived() { remove(namer.getReceivedFilename()); }

	void TextFileApprover::reportToUser() const {
		ensureApprovedFile();
		startDiff(namer.getReceivedFilename(), namer.getApprovedFilename());
	}
	void TextFileApprover::ensureApprovedFile() const {
		if(approvedContents.length() == 0) {   // When string is empty, we don't know whether the file actually exists or not.
			makeFileWithContents(namer.getApprovedFilename(), "");
		}
	}


	// Implementations:

	std::string TextFileApprover::getFileContents(const std::string& filename) const {
		return TextFileUtilities::getFileContentsWithNewlines(filename);
	}
	void TextFileApprover::makeFileWithContents(const std::string& filename, const std::string& contents) const {
		TextFileUtilities::makeFileWithContents(filename, contents);
	}
	void TextFileApprover::remove(const std::string& filename) const {
		TextFileUtilities::removeFile(filename.c_str());
	}

	void TextFileApprover::startDiff(const std::string& lhsFilename, const std::string& rhsFilename) const {
		char key = 'm';
		auto mergeProgram = options.at(key);
		//if (!exists(mergeProgram)) {
		//	return;
		//}
		auto argv = { mergeProgram, lhsFilename, rhsFilename };
		std::string startCommand = TextFileUtilities::createWindowsCommandLine(argv);
		launch(startCommand);
	}
	void TextFileApprover::launch(const std::string& startCommand) const {
		system(startCommand.c_str());
	}

	std::string TextFileApprover::makeBoundedContents(const std::string& contents) {
		static std::string bounds = "\n--\n";
		std::stringstream out;
		out << bounds << contents << bounds;
		return out.str();
	}

	namespace TextFileUtilities {
		bool fileExists(const std::string& filename) {
			std::ifstream ifile;
			ifile.open(filename.c_str(), std::ios::in);
			bool ret = ifile.is_open();
			ifile.close();
			return ret;
		}
		void removeFile(const std::string& filename) {
			std::remove(filename.c_str());
		}
		std::string getFileContentsWithNewlines(const std::string& filename) {
			std::ifstream ifs(filename, std::ios::in | std::ios::ate);
			if(!ifs.good()) { return ""; }

			auto fileSize = static_cast<int>(ifs.tellg());
			ifs.seekg(0, std::ios::beg);

			std::string ret;
			ret.reserve(fileSize);

			std::string str;
			// Read the next line from File until it reaches the end.
			while(std::getline(ifs, str)) {
				if (str.size() && str.back() == '\r') {
					str = str.substr(0, str.size() - 1);
				}
				ret += str + '\n';
			}
			return ret;
		}
		void makeFileWithContents(const std::string& filename, const std::string& contents) {
			std::ofstream file(filename);
			file << contents;
		}
		std::string createWindowsCommandLine(const std::vector<std::string>& argv) {
			std::string command = std::accumulate(argv.begin(), argv.end(), std::string(""), [](std::string total, std::string next_part) {return total + " " + "\"" + next_part + "\""; });
			std::string startCommand = "start \"\" /W" + command;
			return startCommand;
		}
	}
}
