#include "tileset.h"
#include <cassert>


void Tileset::resize(std::size_t n) {
	tiles_.resize(n);
}

void Tileset::create_from_texture(const sf::Vector2i &tile_size, int columns, int spacing, int n_tiles, const sf::Texture *texture) {
	tiles_.reserve(n_tiles);
	int rows = (n_tiles + columns - 1) / columns;

	for (int i = 0; i < rows; ++i) 
		for (int j = 0; j < columns; ++j) {
			add_tile({ j * (tile_size.x + spacing), i * (tile_size.y + spacing), tile_size.x, tile_size.y }, texture);
		}
}

void Tileset::add_tile(sf::IntRect &&texture_rect, const sf::Texture *texture) {
	if (texture_rect.width && texture_rect.height)
		tile_size_ = { texture_rect.width, texture_rect.height };
	tiles_.emplace_back(std::move(texture_rect), texture);
}

std::size_t Tileset::count_tiles() const { return tiles_.size(); }

Tile &Tileset::tile(std::size_t tile_id) {
	assert(tile_id < count_tiles());
	return tiles_[tile_id];
}
const Tile &Tileset::tile(std::size_t tile_id) const {
	assert(tile_id < count_tiles());
	return tiles_[tile_id];
}
