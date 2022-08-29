//
// Created by ztk on 2022/8/29.
//

#pragma once

#include <uv.h>
#include <optional>

namespace sw::asyncuv {

class Error;

typedef std::optional<Error> MaybeError;


class Error : std::exception {
private:
	int val;
public:
	explicit Error(int val) : val(val) {}

	[[nodiscard]] const char* what() const noexcept override {
		return uv_strerror(val);
	}

	static void must(int val) {
		if (val) {
			throw Error(val);
		}
	}

	static MaybeError maybe(int val) {
		if (val) {
			return Error(val);
		}
		return {};
	}
};


} // namespace sw::asyncuv
