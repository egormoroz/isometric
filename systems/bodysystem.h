#pragma once

#include "../ecs/ecs.h"
#include <SFML/Graphics/Rect.hpp>

class GridSystem;

class BodySystem : public System<BodySystem> {
	const GridSystem &gs_;
	std::vector<Entity> candidates_;

	void detect_collisions(EventManager &events);
public:
	BodySystem(const GridSystem &gs);
	~BodySystem() = default;

	virtual void update(EntityManager &es, EventManager &events, float delta_time) override;
};

