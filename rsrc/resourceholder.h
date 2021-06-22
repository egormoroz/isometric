#pragma once

#include <unordered_map>
#include <memory>
//#include "../util/optionalref.h"
#include "../util/noncopyable.h"
#include <cassert>

template<typename Identifier, typename Resource, typename Derived = void>
class ResourceHolder : public NonCopyable {
	std::unordered_map<Identifier, std::unique_ptr<Resource>> data_;
public:
	ResourceHolder() = default;
	~ResourceHolder() = default;

	void add(Identifier &&id, std::unique_ptr<Resource> &&res_ptr) {
		assert(!this->contains(id));
		data_.emplace(std::move(id), std::move(res_ptr));
	}

	void remove(const Identifier &id) {
		auto found = data_.find(id);
		assert(found != data_.end());
		data_.erase(found);
	}

	bool contains(const Identifier &id) const {
		return data_.count(id);
	}

	template<typename ...Args>
	Resource* get_or_load(const Identifier &id, Args && ...args) {
		if (!contains(id)) {
			if (!static_cast<Derived*>(this)->load_file(std::forward<Args>(args)...))
				return nullptr;
		}
		return this->get(id);
	}

	Resource* get(const Identifier &id) {
		auto found = data_.find(id);
		if (found == data_.end())
			return nullptr;
		return found->second.get();
	}

	void clear() {
		data_.clear();
	}

	const Resource* get(const Identifier &id) const {
		auto found = data_.find(id);
		if (found == data_.end())
			return nullptr;
		return found->second.get();
	}
};
