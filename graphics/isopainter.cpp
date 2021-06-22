#include "isopainter.h"
#include <SFML/Graphics/RenderTarget.hpp>





void IsoPainter::flush()
{
	if (!vertices_.empty()) {
		target_->draw(vertices_.data(), vertices_.size(), sf::Quads, sf::RenderStates(texture_));
		vertices_.clear();
	}
	texture_ = nullptr;
}

void IsoPainter::begin(sf::RenderTarget & target)
{
	target_ = &target;
}

void IsoPainter::end()
{
	this->flush();
	target_ = nullptr;
}

void IsoPainter::reserve(int n_vertices)
{
	vertices_.reserve(n_vertices);
}

void IsoPainter::append_vertices(const sf::Vertex *vertices, std::size_t n) {
	vertices_.insert(vertices_.end(), vertices, vertices + n);
}

void IsoPainter::draw_sprite(const sf::Vector2f &pos, const sf::IntRect &texture_rect, const sf::Texture *texture,
							 float origin_x, float origin_y) {
	if (texture != texture_) {
		this->flush();
		texture_ = texture;
	}
	float left = pos.x - pos.y - origin_x,
		top = (pos.x + pos.y) / 2 - origin_y,
		right = left + texture_rect.width, bottom = top + texture_rect.height,
		tx_left = texture_rect.left, tx_top = texture_rect.top,
		tx_right = tx_left + texture_rect.width, tx_bottom = tx_top + texture_rect.height;
	vertices_.emplace_back(sf::Vector2f(left, top), sf::Vector2f(tx_left, tx_top));
	vertices_.emplace_back(sf::Vector2f(right, top), sf::Vector2f(tx_right, tx_top));
	vertices_.emplace_back(sf::Vector2f(right, bottom), sf::Vector2f(tx_right, tx_bottom));
	vertices_.emplace_back(sf::Vector2f(left, bottom), sf::Vector2f(tx_left, tx_bottom));
}
