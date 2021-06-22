#pragma once

#include "../util/defines.h"
#include <cassert>

class Entity {
	friend class EntityManager;

	EntityManager *manager_;
	EntityID id_;
public:
	Entity() = delete;
	~Entity() = default;

	bool valid() const { return manager_ != nullptr; }
	void invalidate() { manager_ = nullptr; }

	void destroy() {
		assert(valid());
		manager_->destroy(id_);
		invalidate();
	}

	EntityID id() const { return id_; }

	template<typename C, typename ...Args>
	ComponentHandle<C> assign(Args && ...args) {
		assert(valid());
		manager_->assign_component<C>(id_, std::forward<Args>(args)...);
		return manager_->get_component<C>(id_);
	}

	template<typename C>
	void remove_component() {
		assert(valid());
		manager_->destroy_component<C>(id_);
	}

	template<typename C>
	ComponentHandle<C> component() {
		assert(valid());
		return manager_->get_component<C>(id_);
	}

private:
	Entity(EntityManager *manager, EntityID id)
		: manager_(manager), id_(id)
	{}
};
