//
// Created by ztk on 2022/8/23.
//

#pragma once

#include "./printable.h"
#include <cstring>

namespace sw {

class SimpleBitmap : Printable {
public:
	typedef uint64_t UT;

private:
	UT* _ptr = nullptr;
	size_t _size = 0;


public:
	SimpleBitmap() = default;

	virtual ~SimpleBitmap() { std::free(_ptr); }

	[[nodiscard]] inline UT size() const { return _size; }

	[[nodiscard]] inline bool has(UT val) const {
		auto q = val / 64;
		if (q >= _size) return false;
		return _ptr[q] & (UT(1) << (val % 64));
	}

	inline void add(UT val) {
		auto q = preallocate(val);
		_ptr[q] |= (UT(1) << (val % 64));
	}

	inline void del(UT val) {
		auto q = val / 64;
		if (q >= _size) return;

		_ptr[q] &= ~((UT(1) << (val % 64)));
	}

	UT preallocate(UT val) {
		auto q = val / 64;
		if (q + 1 > _size) {
			auto nptr = reinterpret_cast<UT*>(std::calloc(q + 1, sizeof(UT)));
			std::memcpy(nptr, _ptr, _size * sizeof(UT));
			std::free(_ptr);
			_ptr = nptr;
			_size = q + 1;
		}
		return q;
	}

	[[nodiscard]] std::string printable_string() const override {
		std::string buf;
		buf.append(fmt::format("SimpleBitmap(size: {})", _size));
		return buf;
	}
};

} // namespace sw
