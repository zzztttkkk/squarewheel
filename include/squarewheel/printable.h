//
// Created by ztk on 2022/8/22.
//

#include <fmt/format.h>

namespace sw {

class Printable {
public:
	[[nodiscard]] virtual std::string printable_string() const = 0;
};

}

//template<>
//struct fmt::formatter<sw::Printable> {
//	template<typename ParseContext>
//	constexpr auto parse(ParseContext& ctx) {
//		return ctx.begin();
//	}
//
//	template<typename FormatContext>
//	auto format(const sw::Printable& obj, FormatContext& ctx) const -> decltype(ctx.out) {
//		return fmt::format_to(ctx.out(), "{}", obj.printable_string());
//	};
//};