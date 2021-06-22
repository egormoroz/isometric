#pragma once

#include "componentspool.h"
#include "entitydatapool.h"
#include "helper.h"
#include <stack>


class EntityManager : public NonCopyable {
	friend class Entity;
	template<typename C> friend class ComponentHandle;

	ComponentsPool components_;
	EntityDataPool entity_data_;
	std::stack<EntityID> free_ids_;
public:
	EntityManager();
	~EntityManager() = default;

	Entity create();

	void destroy(EntityID id);

	Entity get(EntityID id);

	template<typename ...Cs, typename Predicate>
	void each(Predicate p) {
		for (EntityID id = 0; id < entity_data_.size(); ++id) {
			if (entity_data_.alive(id) && 
				test_variadic(entity_data_.has_component(id, Cs::family())...)) {
				p(Entity(this, id), get_component<Cs>(id)...);
			}
		}
	}

private:
	template<typename C, typename ...Args>
	void assign_component(EntityID id, Args && ...args) {
		entity_data_.set_component(id, C::family(), true);
		components_.assign<C>(id, std::forward<Args>(args)...);
	}

	template<typename C>
	void destroy_component(EntityID id) {
		assert(entity_data_.has_component(id, C::family()));
		components_.destroy<C>(id);
		entity_data_.set_component(id, C::family(), false);
	}

	template<typename C>
	C* get_component_ptr(EntityID id) {
		return components_.get<C>(id);
	}

	template<typename C>
	const C* get_component_ptr(EntityID id) const {
		return components_.get<C>(id);
	}

	template<typename C>
	ComponentHandle<C> get_component(EntityID id ) {
		return ComponentHandle<C>(this, id);
	}
};
