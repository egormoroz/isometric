#pragma once

#include <string>
#include "resourceholder.h"
#include "tileset.h"

class TextureHolder;

class TilesetHolder : public ResourceHolder<std::string, Tileset, TilesetHolder> {
public:
	TilesetHolder() = default;
	~TilesetHolder() = default;

	//generates identifier automatically
	bool load_file(const std::string &path, TextureHolder &txs);
};

