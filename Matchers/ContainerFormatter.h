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
#ifndef CONTAINERFORMATTER_H_388AF56A_C371_41E7_B6A1_5C285DC157AB
#define CONTAINERFORMATTER_H_388AF56A_C371_41E7_B6A1_5C285DC157AB

#include "TableFormatting/TableFormatting.h"

namespace gppUnit {
	template<class STREAM, class DELIM>
	struct format_iterator_t: std::iterator<std::output_iterator_tag, void, void, void, void> {
		format_iterator_t(STREAM& strm, const DELIM* delim): strm(&strm),
			delim(delim),
			first(true)
		{}
		template<class VALUE>
		format_iterator_t& operator=(const VALUE& value) {
			if(!first) {
				if(delim != 0) {
					*strm << delim;
				}
			}
			first = false;
			*strm << value;
			return (*this);
		}
		// operator* and operator++ to satisfy std::copy
		format_iterator_t& operator*() { return (*this);	}
		format_iterator_t& operator++() { return (*this); }

	protected:
		STREAM* strm;
		const DELIM* delim;
		bool first;
	};
	template<class STREAM, class DELIM>
	format_iterator_t<STREAM, DELIM> format_iterator(STREAM& strm, const DELIM* delim) { return format_iterator_t<STREAM, DELIM>(strm, delim); }

	struct ContainerFormatter {
		template<typename T>
		static void describe(const T& value, TableFormatter& strm) {
			if(value.size() == 0) {
				strm << "an empty container";
			} else {
				strm << '[';
				std::copy(value.begin(), value.end(), format_iterator(strm, " "));
				strm << ']';
			}
		}
	};
}
#endif // CONTAINERFORMATTER_H_388AF56A_C371_41E7_B6A1_5C285DC157AB
