#include "entitydatapool.h"


void EntityDataPool::expand(std::size_t n) {
	pool_.expand(n);
}

void EntityDataPool::create(EntityID id) {
	new (pool_.get(id)) EntityData();
	++alive_counter_;
}

void EntityDataPool::destroy(EntityID id) {
	assert(alive(id));
	pool_.destroy(id);
	--alive_counter_;
}

void EntityDataPool::set_component(EntityID id, BaseComponent::Family component_id, bool b) {
	assert(alive(id));
	this->get(id).mask.set(component_id, b);
}

bool EntityDataPool::has_component(EntityID id, BaseComponent::Family component_id) const {
	assert(alive(id));
	return this->get(id).mask.test(component_id);
}

size_t EntityDataPool::size() const {
	return pool_.size();
}

bool EntityDataPool::alive(EntityID id) const {
	if (id >= pool_.size())
		return false;
	return this->get(id).alive;
}

size_t EntityDataPool::count_alive() const {
	return alive_counter_;
}

EntityDataPool::EntityData & EntityDataPool::get(EntityID id) {
	return *static_cast<EntityData*>(pool_.get(id));
}

const EntityDataPool::EntityData & EntityDataPool::get(EntityID id) const {
	return *static_cast<const EntityData*>(pool_.get(id));
}
