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
#include "TableFormatting/Column.h"

#include "AutoRun.h"

namespace ColumnElements {
	using gppUnit::equals;

	class PublicInterface: public Auto::TestCase {
		void test(void) {
			gppUnit::Column c("foo");

			confirm.that(c.toString(), equals("foo"));
			confirm.that(c.size(), equals(3));
			confirm.that(c.toString(6), equals("foo   "));
		}
	} GPPUNIT_INSTANCE;
	class Copying: public Auto::TestCase {
		void TestEquals(gppUnit::Column copied, const gppUnit::Column& refd) {
			confirm.that(copied.toString(), equals(refd.toString()));
		}
		void test(void) {
			gppUnit::Column c("foo");
			gppUnit::Column d("foo");
			gppUnit::Column f("bar");

			confirm.that(c.toString(), equals(d.toString()));
			confirm.that(c.toString(), !equals(f.toString()), "Columns c(foo) and f(bar) are not equal");

			TestEquals(c, d);
		}
	} GPPUNIT_INSTANCE;
}
