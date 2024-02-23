#pragma once
#if (defined(__GNUC__) && __GNUC__ >= 13) ||                                             \
	(defined(__clang__) && (__clang_major__ >= 14))
// if gcc>=13 or clang>=14, it supports std::format
#include <format>
using std::format;

#else
// Compiler does not support std::format
// We have to impliment it ourselves
// Code from: https://github.com/xhawk18/50print/blob/master/mini_format.h
// Thanks for the origin author.
#include <functional>
#include <sstream>
#include <vector>

using arg_builder_t = std::function<void(std::ostream&)>;
inline std::string
mini_format_impl(const std::string& fmt, const std::vector<arg_builder_t>& arg_builders) {
	std::ostringstream ss;
	size_t			   start = 0;
	for (size_t i = 0; i < arg_builders.size(); ++i) {
		std::size_t pos = fmt.find("{}", start);
		if (pos == std::string::npos) break;
		ss << fmt.substr(start, pos - start);
		arg_builders[i](ss);
		start = pos + 2 /* length of "{}" */;
	}
	ss << fmt.substr(start);
	return ss.str();
}

template <typename Arg> inline arg_builder_t mini_format_arg(const Arg& arg) {
	return [&arg](std::ostream& ss) { ss << arg; };
}

template <typename... Args>
std::string format(const std::string& fmt, const Args&... args) {
	return mini_format_impl(fmt, {mini_format_arg(args)...});
}
#endif
