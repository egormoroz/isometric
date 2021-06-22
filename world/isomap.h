#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <pugixml.hpp>
#include "../rsrc/tileatlas.h"
#include "../util/noncopyable.h"
#include <functional>

class TextureHolder;
class TilesetHolder;

class IsoMap
	: public NonCopyable {

	struct MyTile {
		MyTile() = default;
		MyTile(int i, int h)
			: id(i), height(h)
		{}
		int id = 0;
		int height = 0;
	};

	TileAtlas atlas_;
	std::vector<std::vector<MyTile>> data_;
	sf::Vector2f iso_tile_size_;
	float tile_size_;
	int width_ = 0, height_ = 0;

public:
	IsoMap() = default;
	~IsoMap() = default;

	bool load_file(const std::string &path, TilesetHolder &tsh, TextureHolder &txh,
				   std::function<void(pugi::xml_object_range<pugi::xml_named_node_iterator>)> objects_handler);

	const sf::Vector2f & iso_tile_size() const;
	float tile_size() const;

	const Tile &tile_by_id(int id) const;

	const std::vector<MyTile>& node(int x, int y) const;
	std::vector<MyTile>& node(int x, int y);

	void clear();

	int width() const;
	int height() const;
};

