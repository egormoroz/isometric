#pragma once

#include <vector>
#include <cassert>
#include <memory>
#include "../util/noncopyable.h"

class BasePool : public NonCopyable {
	const size_t chunk_size_;
	const size_t element_size_;
	size_t size_ = 0;
	size_t capacity_ = 0;
	std::vector<std::unique_ptr<char[]>> blocks_;
public:
	BasePool(size_t element_size, size_t chunk_size = 8128)
		: element_size_(element_size), chunk_size_(chunk_size)
	{}
	virtual ~BasePool() = default;

	size_t capacity() const { return capacity_; }
	size_t size() const { return size_; }
	size_t chunks() const { return blocks_.size(); }

	void expand(size_t n) {
		if (n >= size_) {
			if (n >= capacity_)
				reserve(n);
			size_ = n;
		}
	}

	void reserve(size_t n) {
		while (capacity_ < n) {
			auto chunk = std::unique_ptr<char[]>(new char[element_size_ * chunk_size_]);
			blocks_.emplace_back(std::move(chunk));
			capacity_ += chunk_size_;
		}
	}

	void *get(size_t el) {
		assert(el < size_);
		return blocks_[el / chunk_size_].get() + (el % chunk_size_) * element_size_;
	}

	const void *get(size_t el) const {
		assert(el < size_);
		return blocks_[el / chunk_size_].get() + (el % chunk_size_) * element_size_;
	}

	virtual void destroy(size_t idx) = 0;

	void destroy_all() {
		for (size_t i = 0; i < size_; ++i) {
			this->destroy(i);
		}
	}
};

template<typename T, size_t ChunkSize = 8128>
class Pool : public BasePool {
public:
	Pool() : BasePool(sizeof(T), ChunkSize) {}

	virtual void destroy(size_t el) override {
		assert(el < this->size());
		T *ptr = static_cast<T*>(this->get(el));
		ptr->~T();
	}

};