#pragma once

#include <vector>

class Tileset;
struct Tile;

class TileAtlas {
	struct AtlasTile {
		AtlasTile() = default;
		AtlasTile(const Tileset *ts, size_t id)
			: ts(ts), id(id) {}
		const Tileset *ts = nullptr;
		size_t id = 0;
	};
	std::vector<AtlasTile> tiles_;
public:
	TileAtlas() = default;
	~TileAtlas() = default;
	
	void clear();

	void append_tileset(const Tileset &ts);

	const Tile& tile(size_t id) const;
};

