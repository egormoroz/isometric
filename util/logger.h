#pragma once

#include <iostream>

static void write_to_log(const char *format) {
	std::clog << format;
}

template<typename T, typename... Args>
void write_to_log(const char *format, const T &value, const Args & ...args) {
	for (; *format; ++format) {
		if (*format == '%') {
			std::clog << value;
			write_to_log(format + 1, args...);
			return;
		}
		std::clog << *format;
	}
}
