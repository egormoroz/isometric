#pragma once

#include "resourceholder.h"
#include "../graphics/animation.h"
#include <string>

class TextureHolder;

class AnimationHolder : public ResourceHolder<std::string, AnimationGroup, AnimationHolder> {
public:
	bool load_file(const std::string & path, TextureHolder &txs);
};
