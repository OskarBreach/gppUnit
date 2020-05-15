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
#ifndef AUTORUN_H_B72BA09A_909E_4283_855E_75BDE1121091
#define AUTORUN_H_B72BA09A_909E_4283_855E_75BDE1121091

#include "src/TestCaseBase.h"
#include "src/AutoTest.h"
#include "Matchers/ProxyTypeSpecializations.h"
#include "Matchers/MatcherSpecializations.h"

namespace Auto {
	class TestCase: public gppUnit::TestCaseBase, gppUnit::AutoTest {};
}

#ifndef _CONCAT
#define  _CONCAT(a, b) _XCAT(a, b)
#define  _XCAT(a, b)   a ## b
#endif

#define GPPUNIT_INSTANCE  _CONCAT(gppUnit_Instance_,__LINE__)


/*
Code Sample of a complete new module with a failing test

#include "AutoRun.h"

namespace {
    using gppUnit::equals;
	class Test: public Auto::TestCase{
		void test(){
			expect.fail("First Failing Test");
		}
	}GPPUNIT_INSTANCE;
}

*/
#endif // AUTORUN_H_B72BA09A_909E_4283_855E_75BDE1121091
