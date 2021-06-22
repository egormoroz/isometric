#pragma once

#include "../ecs/ecs.h"
#include <SFML/Graphics/Rect.hpp>

struct CollisionEvent : public Event<CollisionEvent> {
	explicit CollisionEvent(Entity left, const sf::FloatRect &lbounds, 
							Entity right, const sf::FloatRect &rbounds)
		: left(left), right(right), lbounds(lbounds), rbounds(rbounds)
	{}

	Entity left, right;
	sf::FloatRect lbounds, rbounds;
};