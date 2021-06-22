#include "RenderSystem.h"
#include "../ecs/ecs.h"
#include "../components/skin.h"
#include "../components/body.h"
#include "../rsrc/animationholder.h"
#include "../rsrc/textureholder.h"
#include "../graphics/isoscene.h"

#include "../util/logger.h"

RenderSystem::RenderSystem(const TextureHolder &textures, const AnimationHolder &animations, IsoScene &scene)
	: textures_(textures), animations_(animations), scene_(scene)
{
}

void RenderSystem::update(EntityManager & es, EventManager &events, float delta_time)
{
	es.each<Skin, Body>([this, delta_time](Entity e, Skin::Handle skin, Body::Handle body) {
		if (!skin->animation) {
			//if the entity hasn't got animation just skip it
			return;
		}
		const AnimationGroup &anim_grp = *skin->animation;
		const Animation &anim = anim_grp[body->direction];
		auto &elapsed_time = skin->elapsed_time;
		skin->elapsed_time += sf::seconds(delta_time);
		if (skin->elapsed_time >= anim.duration()) {
			++skin->current_frame;
			skin->elapsed_time = sf::Time::Zero;
		}
		if (skin->current_frame >= anim.count_frames())
			skin->current_frame = 0;

		auto &frame = anim.frame(skin->current_frame);

		const sf::FloatRect &bounds = body->bounds;
		scene_.put_object(body->bounds, frame.texture, frame.texture_rect);
	});
}
