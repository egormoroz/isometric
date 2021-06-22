#pragma once

#include "../rsrc/textureholder.h"
#include "../rsrc/tilesetholder.h"
#include "../rsrc/animationholder.h"

#include "../graphics/isoscene.h"
#include "../graphics/isoselection.h"
#include "isomap.h"
#include "gridsystem.h"
#include "../ecs/ecs.h"

enum class Resources {
	ANIMATIONS, MAPRESOURCES, EVERYTHING
};

class World {
	EntityManager es_;
	SystemManager systems_;
	EventManager events_;

	AnimationHolder animations_;
	TextureHolder textures_;
	TilesetHolder tilesets_;

	GridSystem gs_;

	Entity player_;
	IsoSelection selection_;

	IsoMap map_;
	IsoScene scene_;
	sf::RenderTarget &target_;


	void handle_player_input();
public:
	World(sf::RenderTarget &target);

	void initialize();

	void free_resources(Resources rc);

	bool load_animations(const std::string &anims_path);
	bool load_map(const std::string &map_path);

	void update(float dt);
	void draw();
};

