#include "world.h"
#include "../components/body.h"
#include "../components/skin.h"
#include "../systems/bodysystem.h"
#include "../systems/rendersystem.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "areas/solidarea.h"

#include "../util/logger.h"



void World::initialize()
{
	sf::View view({ 50.f, 200.f }, { 1600.f, 900.f });
	view.zoom(0.5f);
	target_.setView(view);
	systems_.add<BodySystem>(gs_);
	systems_.add<RenderSystem>(textures_, animations_, scene_);
	scene_.set_size(20, 20);


	player_.assign<Skin>(animations_.get("stay"));
	auto body = player_.assign<Body>().get();
	body->bounds.width = map_.tile_size();
	body->bounds.height = map_.tile_size();

	selection_.set_fill_color(sf::Color::Transparent);
	selection_.set_outline_color(sf::Color::Red);
	selection_.set_size(map_.iso_tile_size().x, map_.iso_tile_size().y);
	selection_.set_outline_thickness(2.f);
}

void World::free_resources(Resources rc)
{
	switch (rc) {
	case Resources::ANIMATIONS:
		animations_.clear();
		break;
	case Resources::MAPRESOURCES:
		tilesets_.clear();
		gs_.clear();
		map_.clear();
		break;
	case Resources::EVERYTHING:
		animations_.clear();
		textures_.clear();
		tilesets_.clear();
		gs_.clear();
		map_.clear();
		break;
	default:
		break;
	}
}

bool World::load_animations(const std::string & anims_path) {
	return animations_.load_file(anims_path, textures_);
}

bool World::load_map(const std::string & map_path) {
	this->free_resources(Resources::MAPRESOURCES);
	return map_.load_file(map_path, tilesets_, textures_, 
						  [this](pugi::xml_object_range<pugi::xml_named_node_iterator> grps) { 
		gs_.set_size(map_.width() * map_.tile_size(), map_.height() * map_.tile_size());
		for (auto &grp_node : grps) {
			if (strcmp(grp_node.attribute("name").as_string(), "solid") == 0) {
				for (auto &obj_node : grp_node.children("object")) {
					gs_.add_area<SolidArea>(obj_node.attribute("x").as_float(), obj_node.attribute("y").as_float(),
											obj_node.attribute("width").as_float(), obj_node.attribute("height").as_float());
				}
			}
		}
	});
}

void World::handle_player_input()
{
	using Kbd = sf::Keyboard;
	Body *body = player_.component<Body>().get();
	Skin::Handle skin = player_.component<Skin>();
	sf::Vector2f &vel = body->velocity;
	bool left = Kbd::isKeyPressed(Kbd::Left), right = Kbd::isKeyPressed(Kbd::Right),
		down = Kbd::isKeyPressed(Kbd::Down), up = Kbd::isKeyPressed(Kbd::Up);
	constexpr float MOVESPEED = 50.f;
	vel = {};
	skin->animation = animations_.get("stay");
	if (left) {
		vel.x = -MOVESPEED;
	} else if (right) {
		vel.x = MOVESPEED;
	}
	if (down) {
		vel.y = MOVESPEED;
	} else if (up) {
		vel.y = -MOVESPEED;
	}
	if (vel.x || vel.y) {
		skin->animation = animations_.get("walk");
	}
}

World::World(sf::RenderTarget &target)
	: es_(), systems_(es_, events_), target_(target), player_(es_.create())
{
	scene_.set_map(map_);
}


void World::update(float dt)
{
	scene_.reset();
	handle_player_input();
	systems_.update<BodySystem>(dt);
	systems_.update<RenderSystem>(dt);
	auto body = player_.component<Body>();
	selection_.set_position(body->bounds.left, body->bounds.top);
	write_to_log("%, %\n", body->bounds.left, body->bounds.top);
}

void World::draw()
{
	scene_.render(target_);
	target_.draw(selection_);
}
