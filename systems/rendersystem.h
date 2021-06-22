#pragma once

#include "../ecs/system.h"

class TextureHolder;
class AnimationHolder;
class IsoScene;

class RenderSystem : public System<RenderSystem> {
	const TextureHolder &textures_;
	const AnimationHolder &animations_;
	IsoScene &scene_;

public:
	RenderSystem(const TextureHolder &textures, const AnimationHolder &animations, IsoScene &scene);

	virtual void update(EntityManager & es, EventManager &events, float delta_time) override;

};
