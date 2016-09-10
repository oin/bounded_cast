#pragma once
/**
 * C++11 header-only library for conversions between bounded ranges of different types.
 * (C) 2016 Jonathan Aceituno <join@oin.name> (http://oin.name)
 *
 * License: MIT
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <type_traits>
#include <limits>
#include <algorithm>
#include <ratio>

namespace numeric_domain {
/**
 * numeric_domain<T> provides static numeric bounds/range information for type T.
 * See dynamic_domain<T> for a version where bounds are free to change at runtime.
 *
 * Template specializations of this structure must provide the following elements:
 *
 *  - type `value_type`: the type of values dealt with
 *  - `static constexpr const value_type min()`: a constexpr function returning the minimum value in the range.
 *  - `static constexpr const value_type max()`: a constexpr function returning the maximum value in the range.
 *
 * T can be an arithmetic type (i.e., one compatible with std::numeric_limits).
 * However, in order to specify different bounds for the same type (e.g. floats between 0 and 1, and float between -1 and 1), you can provide T as a tag structure (i.e., an empty struct), as well as a valid template specialization.
 */
template <typename T, typename = void>
struct numeric_domain {};

/**
 * Alias for the value type described by numeric_domain<T> (choose whichever one is easier to type).
 *
 * You cannot always assume that the value type of numeric_domain<T> is T, because T might be a tag structure.
 */
template <typename T>
using value_type_of = typename numeric_domain<T>::value_type;

/**
 * Alias for the extent type described by numeric_domain<T> (choose whichever one is easier to type).
 *
 * You cannot assume that the extent type of numeric_domain<T> is T.
 */
template <typename T>
using extent_type_of = decltype(std::declval<value_type_of<T>>() - std::declval<value_type_of<T>>());

/**
 * Return the extent of a numeric_domain type; i.e., the difference between its maximum and its minimum value.
 */
template <typename T>
constexpr extent_type_of<T> extent_of() {
	return numeric_domain<T>::max() - numeric_domain<T>::min();
}

/**
 * Convert a value within specific bounds.
 *
 * The value is clamped if outside (tmin, tmax).
 * It is then rescaled to the range described by umin and uextent.
 */
template <typename U, typename UExtent, typename T, typename TExtent>
U domain_convert(const T t, const T tmin, const T tmax, const TExtent textent, const U umin, const UExtent uextent) {
	T bounded = std::max(tmin, std::min(tmax, t));
	auto scaled = (bounded - tmin) * uextent;
	auto rescaled = umin + scaled / textent;
	return static_cast<U>(rescaled);
}

/**
 * Template specialization of numeric_domain for arithmetic types.
 */
template <typename T>
struct numeric_domain<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
	typedef T value_type;
	static constexpr const value_type min() { return std::numeric_limits<value_type>::min(); }
	static constexpr const value_type max() { return std::numeric_limits<value_type>::max(); }
};

/**
 * A tag for an arithmetic type bounded between two values Min*RatioScaler and Max*RatioScaler.
 */
template <typename T, std::intmax_t Min = std::numeric_limits<T>::min(), std::uintmax_t Max = std::numeric_limits<T>::max(), typename RatioScaler = std::ratio<1, 1>>
struct arithmetic_t {};

/**
 * Template specialization of numeric_domain for arithmetic_t<...> types.
 */
template <typename T, std::intmax_t Min, std::uintmax_t Max, typename RatioScaler>
struct numeric_domain<arithmetic_t<T, Min, Max, RatioScaler>> {
	typedef T value_type;
	static constexpr const value_type min() { return RatioScaler::num * static_cast<value_type>(Min) / RatioScaler::den; }
	static constexpr const value_type max() { return RatioScaler::num * static_cast<value_type>(Max) / RatioScaler::den; }
};

/**
 * Alias for an unsigned arithmetic_t<...> integer type with the given number of bits.
 *
 * For instance, a 12-bit value in a uint16_t may be converted to a float between 0 and 1 by doing: domain_cast<float01, unsigned_int<12>>(value).
 */
template <unsigned int Bits>
using unsigned_int = arithmetic_t<decltype((1 << Bits) - 1), 0, (1 << Bits) - 1>;

/**
 * Alias for a signed arithmetic_t<...> integer type with the given number of bits.
 */
template <unsigned int Bits>
using signed_int = arithmetic_t<decltype(-((1 << Bits) - 1)), -(1 << (Bits - 1)), (1 << (Bits - 1)) - 1>;

/**
 * Alias for float types between 0 and 1.
 */
using float01 = arithmetic_t<float, 0, 1>;
/**
 * Alias for float types between -1 and 1.
 */
using float11 = arithmetic_t<float, -1, 1>;
/**
 * Alias for float types between 0 and 0.5 (shows how to use RatioScaler).
 */
using float_0_and_0_5 = arithmetic_t<float, 0, 1, std::ratio<1,2>>;

/**
 * dynamic_domain<T> provides runtime numeric bounds/range information for type T.
 * See static_domain<T> for a version where bounds are defined at compile time.
 */
template <typename T>
struct dynamic_domain {
	typedef T value_type;
	typedef decltype(std::declval<T>() - std::declval<T>()) extent_type;

	dynamic_domain(value_type m, value_type M) : min(m), max(M) {}
	value_type min;
	value_type max;
	extent_type extent() const { return static_cast<extent_type>(max) - static_cast<extent_type>(min); }
};

/**
 * Create a dynamic domain with the given bounds.
 */
template <typename T>
dynamic_domain<T> make_domain(T min, T max) {
	return dynamic_domain<T>(min, max);
}

/**
 * Create a dynamic domain based on the static information given by arithmetic type or tag class T.
 */
template <typename T>
dynamic_domain<value_type_of<T>> make_domain() {
	return dynamic_domain<value_type_of<T>>(numeric_domain<T>::min(), numeric_domain<T>::max());
}

// Using a functor here should allow an optimization when casting between the same type (partial function template specialization isn't allowed).
template <typename U, typename T>
struct domain_caster {
	value_type_of<U> operator()(const value_type_of<T> value) {
		return domain_convert(value, numeric_domain<T>::min(), numeric_domain<T>::max(), extent_of<T>(), numeric_domain<U>::min(), extent_of<U>());
	}
};
template <typename U>
struct domain_caster<U,U> {
	value_type_of<U> operator()(const value_type_of<U> value) {
		return value;
	}
};

/**
 * Convert a value within numeric_domain<T> to numeric_domain<U>.
 */
template <typename U, typename T>
value_type_of<U> domain_cast(const value_type_of<T> value) {
	return domain_caster<U,T>()(value);
}

/**
 * Convert a value within a given dynamic domain to another dynamic domain.
 */
template <typename DynamicDomainTo, typename DynamicDomainFrom>
const typename DynamicDomainTo::value_type domain_cast(const DynamicDomainTo to, const typename DynamicDomainFrom::value_type value, const DynamicDomainFrom from) {
	return domain_convert(value, from.min, from.max, from.extent(), to.min, to.extent());
}

/**
 * Convert a value within numeric_domain<T> to a given dynamic domain.
 */
template <typename T, typename DynamicDomainTo>
const typename DynamicDomainTo::value_type domain_cast(const DynamicDomainTo to, const value_type_of<T> value) {
	return domain_convert(value, numeric_domain<T>::min(), numeric_domain<T>::max(), extent_of<T>(), to.min, to.extent());
}

/**
 * Convert a value within a given dynamic domain to numeric_domain<U>.
 */
template <typename U, typename DynamicDomainFrom>
const value_type_of<U> domain_cast(const typename DynamicDomainFrom::value_type value, const DynamicDomainFrom from) {
	return domain_cast(make_domain<U>(), value, from);
}

}
