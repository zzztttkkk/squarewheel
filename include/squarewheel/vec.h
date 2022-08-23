//
// Created by ztk on 2022/8/22.
//

#pragma once

#include <cstring>
#include <optional>
#include <functional>

#include "./common.h"
#include "./printable.h"
#include "./bitmap.h"

namespace sw {

template<typename T>
class Vec : Printable {
private:
	T* _ptr = nullptr;
	size_t _size = 0;
	size_t _cap = 0;

	// avoid `constructor` and `destructor` call
	static inline T* allocate(size_t size) { return reinterpret_cast<T*>(std::malloc(size * sizeof(T))); }

	static inline void release(T* ptr) { std::free(ptr); }

	static inline void copy(T* dest, T* src, size_t s) {
		if (s < 1) return;
		std::memcpy(dest, src, sizeof(T) * s);
	}

	inline void grow_to(size_t s) {
		if (s <= _cap) return;
		T* nptr = allocate(s);
		copy(nptr, _ptr, _size);
		release(_ptr);
		_ptr = nptr;
		_cap = s;
	}

	inline void auto_grow() {
		grow_to(_cap == 0 ? 1 : _cap << 1);
	}

	inline void push_unchecked(const T& ele) {
		_ptr[_size] = ele;
		_size++;
	}

public:
	typedef T Ele;
	typedef Vec<T> This;

	Vec() = default;

	explicit Vec(size_t cap) {
		_cap = cap;
		_ptr = allocate(cap);
	}

	Vec(std::initializer_list<T> seq) {
		grow_to(seq.size());
		concat(seq);
	}

	virtual ~Vec() { clear(); };

	[[nodiscard]] inline size_t size() const { return _size; }

	[[nodiscard]] inline size_t capacity() const { return _cap; }

	inline This& push(const T& val) {
		if (_size + 1 > _cap) auto_grow();
		push_unchecked(val);
		return *this;
	}

	inline std::optional<T> pop() {
		if (_size < 1) return {};
		T& temp = _ptr[_size - 1];
		_size--;
		return temp;
	}

#define VecAt(cons) \
        inline cons T& at(size_t pos) cons {\
            if (pos >= _size) throw std::out_of_range(fmt::format("Vec(size: {}), pos: {}", _size, pos));\
            return _ptr[pos];\
        }\

#define VecAtEmpty

	VecAt(VecAtEmpty);

	VecAt(const);

#undef VecAtEmpty
#undef VecAt

	This& removeif(std::function<bool(size_t, const T&)> fn, const bool* break_ptr = nullptr) {
		SimpleBitmap marks;
		marks.preallocate(_size);

		size_t count = 0;
		for (int i = 0; i < _size; ++i) {
			if (fn(i, _ptr[i])) {
				marks.add(i);
				count++;
			}
			if (break_ptr != nullptr && *break_ptr) {
				break;
			}
		}

		if (count >= _size) {
			_size = 0;
			return *this;
		}

		T* nptr = allocate(_size - count);
		size_t j = 0;
		for (size_t i = 0; i < _size; ++i) {
			if (marks.has(static_cast<uint64_t>(i))) continue;
			nptr[j] = _ptr[i];
			j++;
		}
		release(_ptr);
		_ptr = nptr;
		_size -= count;
		_cap = _size;

		return *this;
	}

	This& map(std::function<std::optional<T>(size_t, const T&)> fn, const bool* break_ptr = nullptr) {
		for (int i = 0; i < _size; ++i) {
			std::optional<T> result = fn(i, _ptr[i]);
			if (result.has_value()) {
				_ptr[i] = result.value();
			}
			if (break_ptr != nullptr && *break_ptr) {
				break;
			}
		}
		return *this;
	}

	inline This& concat(const T* ptr, size_t size) {
		grow_to(_size + size);
		std::memcpy(_ptr + _size, ptr, size * sizeof(T));
		_size += size;
		return *this;
	}

	template<typename Seq>
	This& concat(const Seq& seq) {
		auto size = seq.size();
		if (size < 1) return *this;

		if (size > 20 && size > (_size >> 1)) {
			grow_to(_size + size);
			auto cursor = seq.begin();
			auto end = seq.end();
			for (; cursor != end; ++cursor) {
				const T& ele = *cursor;
				push_unchecked(ele);
			}
			return *this;
		}

		auto cursor = seq.begin();
		auto end = seq.end();
		for (; cursor != end; ++cursor) {
			const T& ele = *cursor;
			push(ele);
		}
		return *this;
	}

	inline This& clear() {
		_size = 0;
		return *this;
	}

	[[nodiscard]] std::string printable_string() const override {
		std::string buf;
		buf.reserve(256);
		buf.append(fmt::format("Vec(cap: {}, size: {})[", _cap, _size));
		for (int i = 0; i < _size; ++i) {
			buf.append(fmt::format("{}", _ptr[i]));
			if (i != _size - 1) {
				buf.append(", ");
			}
		}
		buf.push_back(']');
		return buf;
	}
};

}
