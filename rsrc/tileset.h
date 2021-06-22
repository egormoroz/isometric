#pragma once

#include <vector>
#include <SFML/Graphics/Texture.hpp>

struct Tile {
	Tile() = default;
	explicit Tile(sf::IntRect &&rect, const sf::Texture *tx)
		: texture_rect(std::move(rect)), texture(tx)
	{}
	const sf::Texture *texture = nullptr;
	sf::IntRect texture_rect;
};

class Tileset {
	std::vector<Tile> tiles_;
	sf::Vector2i tile_size_;
public:
	Tileset() = default;
	~Tileset() = default;

	void resize(std::size_t n);

	void create_from_texture(const sf::Vector2i &tile_size, int columns, int spacing, int n_tiles, const sf::Texture *texture);

	void add_tile(sf::IntRect &&texture_rect, const sf::Texture *texture);

	std::size_t count_tiles() const;

	Tile &tile(std::size_t tile_id);
	const Tile &tile(std::size_t tile_id) const;
};

