//
// Created by ztk on 2022/8/22.
//

#pragma once

#include <fmt/core.h>
#include <fmt/format.h>
#include <mutex>


namespace sw {

// https://github.com/fmtlib/fmt/issues/2753
// https://stackoverflow.com/a/69647103/6683474
template<typename ...Args>
void print(fmt::format_string<Args...> fmt, Args&& ... args) {
	fmt::print(fmt, std::forward<Args>(args)...);
}

template<typename ...Args>
void println(fmt::format_string<Args...> fmt, Args&& ... args) {
	static std::mutex mtx;
	std::lock_guard<std::mutex> guard(mtx);

	fmt::print(fmt, std::forward<Args>(args)...);
	fmt::print("\r\n");
}

}