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
#include "TableFormatting/TableLine.h"

#include "AutoRun.h"

namespace TestTableLineElements {
	using gppUnit::equals;
	using gppUnit::TableLine;

	class LineHelper: public Auto::TestCase {
		TableLine line;
		std::vector<size_t> sizes;
	protected:
		void givenLine() { line.clear(); }
		void givenSizes() { sizes.clear(); }

		void whenAppend(const std::string& str) { line.append(str); }
		void whenAppend(const TableLine& otherLine) { line.append(otherLine); }
		void whenTab() { line.tab(); }
		void addSize(size_t size) { sizes.push_back(size); }
		void whenUpdateSizes() { line.update(sizes); }

		void thenLineEquals(const std::string& str) {
			std::string msg = "Line should equal: ";
			msg += str;
			confirm.that(line.toString(), equals(str), msg.c_str());
		}
		void sizedLineEquals(const std::string& str) {
			std::string msg = "Line should equal: ";
			msg += str;
			confirm.that(line.toString(sizes), equals(str), msg.c_str());
		}

		void thenSizesHas(size_t elements) {
			expect.that(sizes.size(), equals(elements), "thenSizesHas");
		}
		void thenSizesHasAt(size_t index, size_t value) {
			std::stringstream strm;
			strm << "Index: " << index << " should contain value: " << value;
			confirm.that(sizes[index], equals(value), strm.str().c_str());
		}
	};
	class PublicInterface: public LineHelper {
		void test(void) {
			givenLine();
			thenLineEquals("");

			whenAppend("foo");
			thenLineEquals("foo");

			whenAppend("foo");
			thenLineEquals("foofoo");
		}
	} GPPUNIT_INSTANCE;
	class FormattedOutput: public LineHelper {
		void test(void) {
			givenLine();

			whenAppend("foo");
			whenTab();
			whenAppend("foo");
			thenLineEquals("foofoo");

			givenSizes();
			sizedLineEquals("foofoo");

			addSize(4);
			sizedLineEquals("foo foo");

			addSize(6);
			sizedLineEquals("foo foo");

			givenLine();
			thenLineEquals("");
			sizedLineEquals("");
		}
	} GPPUNIT_INSTANCE;
	class UpdateSizes: public LineHelper {
		void test(void) {
			givenLine();

			whenAppend("foo");
			whenTab();
			whenAppend("doof");

			givenSizes();
			whenUpdateSizes();
			thenSizesHas(2);

			thenSizesHasAt(0, 4);
			thenSizesHasAt(1, 0);
		}
	} GPPUNIT_INSTANCE;
	class LineCopying: public Auto::TestCase {
		std::string copy(const gppUnit::TableLine& line) {
			gppUnit::TableLine line2(line);
			return line2.toString();
		}
		void test(void) {
			gppUnit::TableLine line;
			line.append("line");
			confirm.that(copy(line), equals("line"), "Copy retains contents");
		}
	} GPPUNIT_INSTANCE;
	class LineAssigning: public Auto::TestCase {
		std::string copy(const gppUnit::TableLine& line) {
			gppUnit::TableLine line2;
			line2 = line;
			return line2.toString();
		}
		void test(void) {
			gppUnit::TableLine line;
			line.append("line");
			confirm.that(copy(line), equals("line"), "Copy retains contents");
		}
	} GPPUNIT_INSTANCE;
	class AppendLine: public LineHelper {
		void test(void) {
			givenLine();

			whenAppend("foo");
			whenTab();
			whenAppend("doof");

			TableLine newLine;
			newLine.append("bar");
			newLine.tab();
			newLine.append("zoo");

			whenAppend(newLine);
			thenLineEquals("foodoofbarzoo");
		}
	} GPPUNIT_INSTANCE;
}
