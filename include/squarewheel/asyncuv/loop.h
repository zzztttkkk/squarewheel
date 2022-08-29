//
// Created by ztk on 2022/8/29.
//

#pragma once

#include <uv.h>
#include <optional>

#include "../common.h"
#include "./error.h"

namespace sw::asyncuv {

class Loop {
private:
	uv_loop_t loop{};

public:
	enum class RunMode : int {
		Default = UV_RUN_DEFAULT,
		Once = UV_RUN_ONCE,
		NoWait = UV_RUN_NOWAIT,
	};

	Loop() {
		Error::must(uv_loop_init(&loop));
	}

	MaybeError run(RunMode runMode = RunMode::Default) {
		return Error::maybe(uv_run(&loop, static_cast<uv_run_mode>(runMode)));
	}

	MaybeError close() {
		return Error::maybe(uv_loop_close(&loop));
	}
};

} // namespace sw::asyncuv