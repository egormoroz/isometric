#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../util/defines.h"
#include <pugixml.hpp>


class Animation {
	struct Frame {
		explicit Frame(const sf::IntRect &texture_rect, const sf::Texture *texture)
			: texture_rect(texture_rect), texture(texture)
		{}
		sf::IntRect texture_rect;
		const sf::Texture *texture = nullptr;
	};
	std::vector<Frame> frames_;
	sf::Time duration_;
public:
	Animation() = default;
	~Animation() = default;

	void add_frame(const sf::IntRect &texture_rect, const sf::Texture *texture);
	void clear();

	const Frame &frame(size_t idx) const;
	size_t count_frames() const;

	void set_duration(const sf::Time &duration);
	const sf::Time &duration() const;
};

class AnimationGroup {
	std::array<Animation, 8> animations_;
public:
	AnimationGroup() = default;
	~AnimationGroup() = default;

	Animation& operator[](Direction direction);
	const Animation& operator[](Direction direction) const;
};

