#include "tileatlas.h"
#include "tileset.h"

void TileAtlas::clear() {
	tiles_.clear();
}

void TileAtlas::append_tileset(const Tileset & ts) {
	for (size_t i = 0; i < ts.count_tiles(); ++i) {
		tiles_.emplace_back(&ts, i);
	}
}

const Tile & TileAtlas::tile(size_t id) const {
	return tiles_[id].ts->tile(tiles_[id].id);
}
