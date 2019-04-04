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
#include "AutoMain.h"
#include "ApprovalsTest/CommandLineOptions.h"

#include <iostream>

int main(int argc, char* argv[]) {
	gppUnit::CommandLineOptions options;
	for(int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		if(arg.length() > 1) {
			if(arg[0] == '-') {
				char key = arg[1];
				std::string val = &arg[2];
				if(val.length() > 1) {
					if(val[0] == '=') {
						val.erase(0, 1);
					}
				}
				options[key] = val;
			}
		}
	}

	bool result = gppUnit::AutoMain(options);
	if(options.count('p') > 0) {
		std::cout << "Press return...";		// summ-gcov:ignore
		std::cin.get();						// summ-gcov:ignore
	}
	return !result;
}
