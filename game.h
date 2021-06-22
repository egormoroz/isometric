#pragma once

#include "world/world.h"
#include <SFML/Graphics/RenderWindow.hpp>


class Game {
	sf::RenderWindow window_;
	World world_;

	void process_events();
	void update(float dt);
	void render();
public:
	Game();
	~Game() = default;

	int execute();
};

