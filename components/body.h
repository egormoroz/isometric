#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "../ecs/component.h"

struct Body : public Component<Body> {
	sf::FloatRect bounds;
	sf::Vector2f velocity;
	Direction direction = Direction::EAST;
};
