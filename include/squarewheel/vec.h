//
// Created by ztk on 2022/8/22.
//

#pragma once

#include "./common.h"
#include "./printable.h"
#include <cassert>

namespace sw {

template<typename T>
class Vec : Printable {
private:
	T* _ptr = nullptr;
	size_t _size = 0;
	size_t _cap = 0;

	static inline T* allocate(size_t size) { return new T[size]; }

	static inline void copy(T* dest, T* src, size_t s) {
		if (s < 1) return;
		std::memcpy(dest, src, sizeof(T) * s);
	}

	inline void grow_to(size_t s) {
		if (s <= _cap) return;
		T* nptr = allocate(s);
		copy(nptr, _ptr, _size);
		delete[] _ptr;
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

	[[nodiscard]] size_t size() const { return _size; }

	[[nodiscard]] size_t capacity() const { return _cap; }

	inline void push(const T& val) {
		if (_size + 1 > _cap) auto_grow();
		push_unchecked(val);
	}

	void concat(const T* ptr, size_t size) {
		grow_to(_size + size);
		std::memcpy(_ptr + _size, ptr, size * sizeof(T));
		_size += size;
	}

	template<typename Seq>
	void concat(const Seq& seq) {
		auto size = seq.size();
		if (size < 1) return;

		if (size > 20 && size > (_size >> 1)) {
			grow_to(_size + size);
			auto begin = seq.begin();
			auto end = seq.end();
			for (; begin != end; ++begin) {
				const T& ele = *begin;
				push_unchecked(ele);
			}
			return;
		}

		auto begin = seq.begin();
		auto end = seq.end();
		for (; begin != end; ++begin) {
			const T& ele = *begin;
			push(ele);
		}
	}

	void clear() { _size = 0; }

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