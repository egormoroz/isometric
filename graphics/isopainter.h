#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>

class IsoPainter {
	std::vector<sf::Vertex> vertices_;
	const sf::Texture *texture_ = nullptr;
	sf::RenderTarget *target_ = nullptr;

	void flush();
public:
	IsoPainter() = default;

	void begin(sf::RenderTarget &target);
	void end();

	void reserve(int n_vertices);

	void append_vertices(const sf::Vertex *vertices, std::size_t n);

	void draw_sprite(const sf::Vector2f &pos, const sf::IntRect &texture_rect, const sf::Texture *texture,
					 float origin_x = 0.f, float origin_y = 0.f);

	~IsoPainter() = default;
};

