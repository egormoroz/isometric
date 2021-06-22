#include "bodysystem.h"
#include "../events/collisionevent.h"
#include "../components/body.h"
#include "../world/gridsystem.h"
#include "../world/areas/solidarea.h"

void BodySystem::detect_collisions(EventManager &events)
{
	for (auto &left : candidates_) {
		for (auto &right : candidates_) {
			if (left.id() == right.id())
				continue;
			const sf::FloatRect &left_bounds = left.component<Body>()->bounds,
				right_bounds = right.component<Body>()->bounds;
			if (left_bounds.intersects(right_bounds))
				events.emit<CollisionEvent>(left, left_bounds, right, right_bounds);
		}
	}
}

BodySystem::BodySystem(const GridSystem & gs)
	: gs_(gs)
{
}

void BodySystem::update(EntityManager &es, EventManager &events, float delta_time)
{
	candidates_.clear();

	es.each<Body>([this, delta_time](Entity e, Body::Handle body) {
		const sf::Vector2f &vel = body->velocity;
		sf::FloatRect &bounds = body->bounds;
		auto check_new_pos = [&](const sf::Vector2f &pos) -> bool {
			int counter = 0;
			gs_.each_intersected<SolidArea>(sf::FloatRect(pos.x, pos.y, bounds.width, bounds.height), 
											[&counter](const SolidArea &) { ++counter; });
			return pos.x >= 0.f && pos.y >= 0.f && counter == 0;
		};
		Direction &direction = body->direction;
		if (vel.y > 0.f) {
			direction = Direction::SOUTH;
		} else if (vel.y < 0.f) {
			direction = Direction::NORTH;
		}
		if (vel.x > 0.f) {
			direction = Direction::EAST;
			if (vel.y > 0.f) {
				direction = Direction::SOUTHEAST;
			} else if (vel.y < 0.f) {
				direction = Direction::NORTHEAST;
			}
		} else if (vel.x < 0.f) {
			direction = Direction::WEST;
			if (vel.y > 0.f) {
				direction = Direction::SOUTHWEST;
			} else if (vel.y < 0.f) {
				direction = Direction::NORTHWEST;
			}
		} 

		sf::Vector2f new_pos(bounds.left + vel.x * delta_time, bounds.top);
		if (check_new_pos(new_pos))
			bounds.left = new_pos.x;

		new_pos = { bounds.left, bounds.top + vel.y * delta_time };
		if (check_new_pos(new_pos))
			bounds.top = new_pos.y;

		candidates_.emplace_back(e);
	});

	detect_collisions(events);
}
