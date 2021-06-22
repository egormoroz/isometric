#pragma once

#include "../util/pool.h"
#include "../util/defines.h"
#include <bitset>
#include "component.h"

class EntityDataPool : public NonCopyable {
	struct EntityData {
		using Mask = std::bitset<MAX_COMPONENTS>;
		EntityData()
			: alive(true)
		{}
		Mask mask;
		bool alive;
		~EntityData() {
			alive = false;
			mask.reset();
		}
	};

	Pool<EntityData> pool_;
	std::size_t alive_counter_ = 0;

public:
	EntityDataPool() = default;

	void expand(std::size_t n);

	void create(EntityID id);
	void destroy(EntityID id);

	void set_component(EntityID id, BaseComponent::Family component_id, bool b);
	bool has_component(EntityID id, BaseComponent::Family component_id) const;


	size_t size() const;

	bool alive(EntityID id) const;
	size_t count_alive() const;

private:
	EntityData & get(EntityID id);
	const EntityData & get(EntityID id) const;
};
