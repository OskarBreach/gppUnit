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
#ifndef MATCHERRESULT_H_1E2E601F_63D8_4E29_994C_BAD8A7DC8C0E
#define MATCHERRESULT_H_1E2E601F_63D8_4E29_994C_BAD8A7DC8C0E

#include "TableFormatting/TableFormatting.h"

namespace gppUnit {
	struct MatcherResult {
		bool result;
		TableFormatter strm;

		bool hasActual;
		TableFormatter actualStrm;

		MatcherResult();
		explicit MatcherResult(bool value);
		std::string description() const ;
	};
}

#endif // MATCHERRESULT_H_1E2E601F_63D8_4E29_994C_BAD8A7DC8C0E
