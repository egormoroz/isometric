#pragma once

#include "../util/defines.h"
#include <cassert>

template<typename C>
class ComponentHandle {
	friend class EntityManager;
	EntityID id_;
	EntityManager *es_;
public:
	C *get() {
		assert(es_ != nullptr);
		return es_->get_component_ptr<C>(id_);
	}

	const C *get() const {
		assert(es_ != nullptr);
		return es_->get_component_ptr<C>(id_);
	}

	C* operator->() {
		assert(es_ != nullptr);
		C* ptr = es_->get_component_ptr<C>(id_);
		assert(ptr != nullptr);
		return ptr;
	}

	const C* operator->() const {
		assert(es_ != nullptr);
		const C* ptr = es_->get_component_ptr<C>(id_);
		assert(ptr != nullptr);
		return ptr;
	}

	explicit operator bool() const {
		if (!es_)
			return nullptr;
		return es_->get_component_ptr<C>(id_) != nullptr;
	}

private:
	explicit ComponentHandle(EntityManager *es, EntityID id)
		: es_(es), id_(id)
	{}
};

class BaseComponent {
public:
	using Family = std::uint32_t;

protected:
	static Family family_counter;
};

template<typename Derived>
struct Component : public BaseComponent {
	static Family family() {
		static Family family = family_counter++;
		assert(family < MAX_COMPONENTS);
		return family;
	}
	using Handle = ComponentHandle<Derived>;
};
