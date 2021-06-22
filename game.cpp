#include "game.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

constexpr int frames_per_second = 100;


Game::Game()
	: window_(sf::VideoMode(1600, 900), "Game"), world_(window_)
{
	window_.setFramerateLimit(frames_per_second);
	//load data and initialize....
	assert(world_.load_map("assets/map.tmx") && world_.load_animations("assets/hero.xml"));
	world_.initialize();
}

void Game::process_events()
{
	sf::Event event;
	while (window_.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window_.close();
		} else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				window_.close();
			}
		}
	}
}

void Game::update(float dt)
{
	world_.update(dt);
}

void Game::render()
{
	window_.clear();
	world_.draw();
	window_.display();
}

int Game::execute()
{
	sf::Clock clock;
	while (window_.isOpen()) {
		process_events();
		update(clock.restart().asSeconds());
		render();
	}
	return 0;
}
