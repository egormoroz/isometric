#include "isomap.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cassert>

#include "../rsrc/tilesetholder.h"
#include "../rsrc/textureholder.h"

#include <pugixml.hpp>
#include "../util/stringhelper.h"
#include "../util/logger.h"
#include "../util/base64.h"

const std::vector<IsoMap::MyTile> & IsoMap::node(int x, int y) const { 
	assert(x >= 0 && y >= 0 && x < width_ && y < height_);
	return data_[y * height_ + x]; 
}

std::vector<IsoMap::MyTile> & IsoMap::node(int x, int y) { 
	assert(x >= 0 && y >= 0 && x < width_ && y < height_);
	return data_[y * height_ + x]; 
}

void IsoMap::clear()
{
	data_.clear();
	atlas_.clear();
	iso_tile_size_ = {};
	tile_size_ = 0.f;
	width_ = 0;
	height_ = 0;
}

bool IsoMap::load_file(const std::string &path, TilesetHolder & tsh, TextureHolder & txh,
					  std::function<void(pugi::xml_object_range<pugi::xml_named_node_iterator>)> objects_handler)
{
	std::string local_folder = extract_local_folder(path);

	pugi::xml_document doc;
	auto res = doc.load_file(path.c_str());
	if (!res) {
		write_to_log("could not load %: %\n", path, res.description());
		return false;
	}

	pugi::xml_node map_node = doc.child("map");
	width_ = map_node.attribute("width").as_int();
	height_ = map_node.attribute("height").as_int();
	iso_tile_size_.x = map_node.attribute("tilewidth").as_float();
	iso_tile_size_.y = map_node.attribute("tileheight").as_float();
	tile_size_ = sqrt(iso_tile_size_.x * iso_tile_size_.x + iso_tile_size_.y * iso_tile_size_.y) / 2;

	data_.resize(width_ * height_);

	atlas_.clear();

	for (const pugi::xml_node &ts_node : map_node.children("tileset")) {
		std::string ts_path = local_folder + ts_node.attribute("source").as_string(),
			ts_id = id_by_path(ts_path);
		const Tileset *ts = tsh.get_or_load(ts_id, ts_path, txh);
		if (ts) {
			atlas_.append_tileset(*ts);
		} else {
			write_to_log("could not load tileset %\n", ts_path);
		}
	}


	//I HOPE LAYERS ARE SORTED)))
	int layer_id = 0, height = 0;

	auto put_tile = [&](int x, int y, int tid) {
		if (tid) {
			if (height > 1) {
				x += height - 1;
				y += height - 1;
			}
			node(x, y).emplace_back(tid - 1, height);
		}
	};

	for (auto &layer_node : map_node.children("layer")) {
		pugi::xml_node data_node = layer_node.child("data");
		std::string encoding = data_node.attribute("encoding").as_string(),
			compression = data_node.attribute("compression").as_string();
		height = layer_node.child("properties").find_child_by_attribute("name", "height").attribute("value").as_int();
		const char *data = data_node.text().as_string();

		if (encoding == "csv") {
			for (int y = 0; y < height_ && *data; ++y)
				for (int x = 0; x < width_ && *data; ++x) {
					put_tile(x, y, atoi(data));
					while (*data && *data != ',')
						++data;
					if (*data == ',')
						++data;
				}
		} else if (encoding == "base64") {
			std::string bin_data = [&]() {
				if (compression == "zlib") {
					return base64::decode(data, true);
				} else if (compression.empty()) {
					return base64::decode(data, false);
				} else {
					write_to_log("unsupported compression %\n", compression);
					return std::string();
				}
			}();

			const int *beg = reinterpret_cast<const int*>(bin_data.data());
			const int *end = reinterpret_cast<const int*>(bin_data.data() + bin_data.size());

			int x = 0, y = 0;
			while (beg != end && y < height_) {
				put_tile(x, y, *beg);
				if (++x >= width_) {
					x = 0;
					++y;
				}
				++beg;
			}
		} else {
			write_to_log("unsupported layer data encoding \"%\"\n", encoding);
		}

		++layer_id;
	}

	objects_handler(map_node.children("objectgroup"));

	return true;
}

const sf::Vector2f & IsoMap::iso_tile_size() const { return iso_tile_size_; }

float IsoMap::tile_size() const { return tile_size_; }

const Tile & IsoMap::tile_by_id(int id) const { return atlas_.tile(id); }

int IsoMap::width() const { return width_; }

int IsoMap::height() const { return height_; }
