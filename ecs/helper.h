#pragma once

template<typename T>
constexpr bool test_variadic(const T &t) {
	return t;
}

template<typename T, typename ...Args>
constexpr bool test_variadic(const T &t, const Args & ...args) {
	return t && test_variadic(args...);
}
