#pragma once

#include <cassert>

namespace opt {
	struct None {};

	static const None none;
}

template<typename T>
class OptionalRef {
	T *ptr_;
public:
	OptionalRef(opt::None)
		: ptr_(nullptr) {}
	OptionalRef(T &t)
		: ptr_(&t)
	{}

	T& operator*() {
		assert(ptr_ != nullptr);
		return ptr_;
	}

	T* operator->() {
		assert(ptr_ != nullptr);
		return ptr_;
	}
	explicit operator bool() const {
		return ptr_ != nullptr;
	}
};
