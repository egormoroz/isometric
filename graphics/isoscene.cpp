#include "isoscene.h"
#include "../world/isomap.h"
#include "../rsrc/tileset.h"
#include <SFML/Graphics/RenderTarget.hpp>




std::vector<IsoScene::Item>& IsoScene::items(int x, int y) {
	return dynamic_items_[(y - render_area_.top) * render_area_.height + x - render_area_.left];
}

const std::vector<IsoScene::Item>& IsoScene::items(int x, int y) const {
	return dynamic_items_[(y - render_area_.top) * render_area_.height + x - render_area_.left];
}

void IsoScene::set_start_position(int x, int y)
{
	render_area_.left = x;
	render_area_.top = y;
}

void IsoScene::set_size(int w, int h)
{
	render_area_.width = w;
	render_area_.height = h;
	dynamic_items_.resize(w * h);
}

void IsoScene::set_map(const IsoMap & map)
{
	map_ = &map;
}

void IsoScene::put_object(const sf::FloatRect &bounds,
 const sf::Texture * texture,
						  const sf::IntRect & texture_rect)
{
	if (!map_)
		return;
	int x = static_cast<int>((bounds.left + bounds.width) / map_->tile_size()), 
		y = static_cast<int>((bounds.top + bounds.height) / map_->tile_size());
	if (render_area_.contains(x, y)) {
		items(x, y).emplace_back(sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2), 
								 texture_rect, texture);
	}
}

void IsoScene::reset()
{
	for (auto &i : dynamic_items_) {
		i.clear();
	}
}

void IsoScene::render(sf::RenderTarget & target)
{
	if (!map_)
		return;
	//std::vector<T> usually doesn't reset its capacity => std::vector<T>::clear() - costless
	vertices_.clear();

	int left = std::max(render_area_.left, 0), top = std::max(render_area_.top, 0),
		right = std::min(render_area_.left + render_area_.width, map_->width()),
		bottom = std::min(render_area_.top + render_area_.height, map_->height());


	const sf::Vector2f &tile_size = map_->iso_tile_size();
	auto draw_sprite = [&](const sf::Vector2f &pos, const sf::Texture *texture, 
							  const sf::IntRect &texture_rect, float orig_x = 0.f, float orig_y = 0.f) {
		if (texture != texture_) {
			if (!vertices_.empty()) {
				target.draw(vertices_.data(), vertices_.size(), sf::Quads, sf::RenderStates(texture_));
				vertices_.clear();
			}
			texture_ = texture;
		}
		float left = pos.x - orig_x,
		top = pos.y - orig_y,
		right = left + texture_rect.width, bottom = top + texture_rect.height,
		tx_left = texture_rect.left, tx_top = texture_rect.top,
		tx_right = tx_left + texture_rect.width, tx_bottom = tx_top + texture_rect.height;
		vertices_.emplace_back(sf::Vector2f(left, top), sf::Vector2f(tx_left, tx_top));
		vertices_.emplace_back(sf::Vector2f(right, top), sf::Vector2f(tx_right, tx_top));
		vertices_.emplace_back(sf::Vector2f(right, bottom), sf::Vector2f(tx_right, tx_bottom));
		vertices_.emplace_back(sf::Vector2f(left, bottom), sf::Vector2f(tx_left, tx_bottom));
	};
	auto comp_pos = [&](int x, int y) -> sf::Vector2f {
//		float xx = x * map_->tile_size(), yy = y * map_->tile_size();
//		return { xx - yy, (xx + yy) / 2 };
		return { (x - y) * tile_size.x / 2.f,  (x + y) * tile_size.y / 2 };
	};
	for (int y = top; y < bottom; ++y) {
		for (int x = left; x < right; ++x) {
			auto &node = map_->node(x, y);
			size_t i = 0;
			for (; i < node.size() && !node[i].height; ++i) {
				const Tile &tile = map_->tile_by_id(node[i].id);
				draw_sprite(comp_pos(x, y), tile.texture, tile.texture_rect, map_->iso_tile_size().x / 2,
							tile.texture_rect.height - map_->iso_tile_size().y);
			}
			for (auto &i : items(x, y)) {
				draw_sprite({ i.pos.x - i.pos.y, (i.pos.x + i.pos.y) / 2 }, i.texture, i.texture_rect,
							i.texture_rect.width / 2, i.texture_rect.height);
			}
			for (; i < node.size(); ++i) {
				const Tile &tile = map_->tile_by_id(node[i].id);
				draw_sprite(comp_pos(x - node[i].height + 1, y - node[i].height + 1), tile.texture, tile.texture_rect, 
							map_->iso_tile_size().x / 2, tile.texture_rect.height - map_->iso_tile_size().y);
			}
		}
	}

	if (!vertices_.empty()) {
		target.draw(vertices_.data(), vertices_.size(), sf::Quads, sf::RenderStates(texture_));
		vertices_.clear();
	}
}
