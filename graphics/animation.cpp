#include "animation.h"
#include <cassert>
#include "../rsrc/textureholder.h"
#include "../util/stringhelper.h"

void Animation::add_frame(const sf::IntRect &texture_rect, const sf::Texture *texture) {
	frames_.emplace_back(texture_rect, texture);
}

void Animation::clear() {
	frames_.clear();
	duration_ = sf::Time::Zero;
}

const Animation::Frame & Animation::frame(size_t idx) const {
	assert(idx < count_frames());
	return frames_[idx];
}
size_t Animation::count_frames() const {
	return frames_.size();
}

void Animation::set_duration(const sf::Time &duration) {
	duration_ = duration;
}

const sf::Time &Animation::duration() const {
	return duration_;
}

Animation & AnimationGroup::operator[](Direction direction)
{
	return animations_[static_cast<std::size_t>(direction)];
}

const Animation & AnimationGroup::operator[](Direction direction) const
{
	return animations_[static_cast<std::size_t>(direction)];
}
