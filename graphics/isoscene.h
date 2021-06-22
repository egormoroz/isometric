#pragma once
#include "../util/noncopyable.h"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>

class IsoMap;

class IsoScene : public NonCopyable {
	struct Item {
		Item() = default;
		Item(const sf::Vector2f &pos, const sf::IntRect &tx_rect, const sf::Texture *texture) 
			: pos(pos), texture_rect(tx_rect), texture(texture)
		{}
		sf::Vector2f pos;
		sf::IntRect texture_rect;
		const sf::Texture *texture = nullptr;
	};

	std::vector<sf::Vertex> vertices_;
	const sf::Texture *texture_ = nullptr;

	const IsoMap *map_ = nullptr;
	sf::IntRect render_area_;
	std::vector<std::vector<Item>> dynamic_items_;

	std::vector<Item> &items(int x, int y);
	const std::vector<Item> &items(int x, int y) const;

public:
	IsoScene() = default;
	~IsoScene() = default;

	void set_start_position(int x, int y);
	void set_size(int w, int h);

	void set_map(const IsoMap &map);

	void put_object(const sf::FloatRect &bounds, const sf::Texture *texture, const sf::IntRect &texture_rect);

	void reset();

	void render(sf::RenderTarget &target);
};

