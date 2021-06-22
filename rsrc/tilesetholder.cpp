#include "tilesetholder.h"
#include <pugixml.hpp>
#include "../util/logger.h"
#include "../util/stringhelper.h"
#include "textureholder.h"


bool TilesetHolder::load_file(const std::string &path, TextureHolder &txs) {
	std::string local_folder = extract_local_folder(path);

	pugi::xml_document doc;
	auto res = doc.load_file(path.c_str());

	if (!res) {
		write_to_log("could not load %: %\n", path, res.description());
		return false;
	}

	pugi::xml_node ts_node = doc.child("tileset");
	if (!ts_node) {
		write_to_log("bad tileset format: could not find tileset node\n");
		return false;
	}


	sf::Vector2i tile_size(ts_node.attribute("tilewidth").as_int(), ts_node.attribute("tileheight").as_int());
	int tile_count = ts_node.attribute("tilecount").as_int(), columns = ts_node.attribute("columns").as_int(),
		spacing = ts_node.attribute("spacing").as_int(), margin = ts_node.attribute("margin").as_int();

	auto get_or_add_texture = [&txs, &local_folder](const pugi::xml_node &image_node, 
													const sf::Vector2i &offset = sf::Vector2i()) -> const sf::Texture* {
		std::string image_path = local_folder + image_node.attribute("source").as_string(),
			texture_id = id_by_path(image_path);

		if (txs.contains(texture_id))
			return txs.get(texture_id);

		sf::Image image;
		if (!image.loadFromFile(image_path)) {
			write_to_log("could not load tileset image %\n", image_path);
			return nullptr;
		}

		pugi::xml_attribute trans_clr = image_node.attribute("trans");
		if (trans_clr) {
			image.createMaskFromColor(sf::Color(trans_clr.as_uint()));
		}

		std::unique_ptr<sf::Texture> texture(new sf::Texture);
		if (!texture->loadFromImage(image, { offset.x, offset.y, image_node.attribute("width").as_int(),
										     image_node.attribute("height").as_int() })) {
			write_to_log("could not load texture from image\n");
			return nullptr;
		}
		const sf::Texture *ptexture = texture.get();
		txs.add(std::move(texture_id), std::move(texture));
		return ptexture;
	};


	std::unique_ptr<Tileset> ts(new Tileset);
	if (columns > 0) {
		//single tileset image
		pugi::xml_node tileoffs_node = ts_node.child("tileoffset");
		sf::Vector2i offset(margin + tileoffs_node.attribute("x").as_int(),
							margin + tileoffs_node.attribute("y").as_int());

		pugi::xml_node image_node = ts_node.child("image");
		if (!image_node) {
			write_to_log("bad tileset format: could not find image node\n");
			return false;
		}

		const sf::Texture *texture = get_or_add_texture(image_node, { offset.x + margin, offset.y + margin });
		if (!texture) 
			return false;

		ts->create_from_texture(tile_size, columns, spacing, tile_count, texture);
	} else {
		ts->resize(tile_count);
		for (const pugi::xml_node &tile_node : ts_node.children("tile")) {
			int id = tile_node.attribute("id").as_int();
			const sf::Texture *texture = get_or_add_texture(tile_node.child("image"), { margin, margin });
			if (!texture)
				return false;
			ts->tile(id).texture = texture;
			ts->tile(id).texture_rect = sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y);
		}
	}

	this->add(id_by_path(path), std::move(ts));
	return true;
}

