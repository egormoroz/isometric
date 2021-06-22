#pragma once

#include <cstdint>
#include <SFML/System/Time.hpp>
#include "../ecs/component.h"

class AnimationGroup;

struct Skin : public Component<Skin> {
	explicit Skin(const AnimationGroup *anim_ptr = nullptr, std::uint32_t current_frame = 0,
				  sf::Time elapsed_time = sf::Time::Zero)
		: animation(anim_ptr), current_frame(current_frame), elapsed_time(elapsed_time)
	{}

	const AnimationGroup *animation;
	std::uint32_t current_frame;
	sf::Time elapsed_time;
};
