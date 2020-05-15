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
#ifndef MATCHERS_H_A1A948B8_4BB3_4E59_8A68_E4E99CC5EEC2
#define MATCHERS_H_A1A948B8_4BB3_4E59_8A68_E4E99CC5EEC2

#include "TableFormatting/TableFormatting.h"
#include "ProxyType.h"
#include "is_not.h"
#include "is_container.h"

#include <string>
#include <sstream>

namespace gppUnit {
	struct is_null {
		template <typename ACTUAL>
		MatcherResult match(const ACTUAL& actual) const {
			MatcherResult result(actual == 0);
			result.strm << "is null";
			return result;
		}

		is_not_t<is_null> operator!() const { return is_not(*this); }
	};

	template <typename T, typename DERIVED>
	struct value_matcher {
		explicit value_matcher(const T& expected): expectedValue(expected) {}

		const DERIVED& derivedMatcher() const { return static_cast<const DERIVED&>(*this); }

		template <typename ACTUAL>
		MatcherResult match(const ACTUAL& actual) const {
			return derivedMatcher()(ProxyValue(actual), ProxyValue(expectedValue));
		}

		is_not_t<DERIVED> operator!() const { return is_not(derivedMatcher()); }

		const T& expectedValue;
	};

	template <typename T>
	struct value_trait_base {
		value_trait_base(const T& actual, const T& expected): actualValue(actual), expectedValue(expected) {}

		const T& actualValue;
		const T& expectedValue;
	};
	template <typename T>
	struct equal_to_trait_base: value_trait_base<T> {
		equal_to_trait_base(const T& actual, const T& expected): value_trait_base<T>(actual, expected) {}

		bool equals() const { return (equal_to_trait_base<T>::actualValue == equal_to_trait_base<T>::expectedValue); }

	};
	template <typename T>
	struct equal_to_trait: equal_to_trait_base<T> {
		equal_to_trait(const T& actual, const T& expected): equal_to_trait_base<T>(actual, expected) {}

		MatcherResult match() {
			MatcherResult result(equal_to_trait<T>::equals());
			result.strm << "'" << equal_to_trait<T>::expectedValue << "'";
			result.actualStrm << "'" << equal_to_trait<T>::actualValue << "'";
			result.hasActual = true;
			return result;
		}
	};

	template <typename T>
	struct equal_to_container_trait;

	template <typename T>
	struct equal_to_t: value_matcher<T, equal_to_t<T> > {
		explicit equal_to_t(const T& expected): value_matcher<T, equal_to_t<T> >(expected) {}

		template<typename V>
		MatcherResult operator()(const V& actual, const V& expected) const {

			typename Loki::Select <
			is_string<V>::result,
			          equal_to_trait<V>,
			          typename Loki::Select <
			          stackoverflow::is_container<V>::result,
			          equal_to_container_trait<V>,
			          equal_to_trait<V>
			          >::Result
			          >::Result helper(actual, expected);
			return helper.match();
		}
	};
	template <typename T>
	equal_to_t<T> equals(const T& expected) { return equal_to_t<T>(expected); }
	template <typename T>
	equal_to_t<T> equal_to(const T& expected) { return equals<T>(expected); }

	template <typename T>
	struct greater_than_t: value_matcher<T, greater_than_t<T> > {
		explicit greater_than_t(const T& expected): value_matcher<T, greater_than_t<T> >(expected) {}

		template<typename V>
		MatcherResult operator()(const V& actual, const V& expected) const {
			MatcherResult result(actual > expected);
			result.strm  << "a value greater than '" << expected << "'";
			return result;
		}
	};
	template <typename T>
	greater_than_t<T> greater_than(const T& expected) { return greater_than_t<T>(expected); }

	template <typename T>
	struct less_than_t: value_matcher<T, less_than_t<T> > {
		explicit less_than_t(const T& expected): value_matcher<T, less_than_t<T> >(expected) {}

		template<typename V>
		MatcherResult operator()(const V& actual, const V& expected) const {
			MatcherResult result(actual < expected);
			result.strm  << "a value less than '" << expected << "'";
			return result;
		}
	};
	template <typename T>
	less_than_t<T> less_than(const T& expected) { return less_than_t<T>(expected); }
}

#endif // MATCHERS_H_A1A948B8_4BB3_4E59_8A68_E4E99CC5EEC2
