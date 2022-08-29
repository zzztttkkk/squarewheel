//
// Created by ztk on 2022/8/22.
//

#pragma once

#include <fmt/format.h>

namespace sw {

class Printable {
public:
	[[nodiscard]] virtual std::string printable_string() const = 0;
};

}

template<typename T>
struct fmt::formatter<
		T,
		std::enable_if<std::is_base_of<sw::Printable, T>::value, char>
> : fmt::formatter<std::string> {
	auto format(const T& obj, fmt::format_context& ctx) {
		return fmt::format_to(ctx.out(), "{}", obj.printable_string());
	};
};
