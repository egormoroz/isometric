#include "entitymanager.h"
#include "entity.h"

EntityManager::EntityManager()
{
	free_ids_.emplace(0);
}

Entity EntityManager::create()
{
	EntityID id = free_ids_.top();
	if (free_ids_.size() == 1) {
		++free_ids_.top();
	}
	entity_data_.expand(id + 1);
	entity_data_.create(id);
	return Entity(this, id);
}

void EntityManager::destroy(EntityID id)
{
	for (BaseComponent::Family i = 0; i < MAX_COMPONENTS; ++i) {
		if (entity_data_.has_component(id, i)) {
			components_.destroy(id, i);
		}
	}
	entity_data_.destroy(id);
	free_ids_.push(id);
}

Entity EntityManager::get(EntityID id)
{
	return Entity(this, id);
}
