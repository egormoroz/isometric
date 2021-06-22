#include "animationholder.h"
#include "textureholder.h"
#include <pugixml.hpp>
#include "../util/stringhelper.h"
#include "../util/logger.h"

bool AnimationHolder::load_file(const std::string & path, TextureHolder & txs)
{
	std::string local_folder = extract_local_folder(path);
	pugi::xml_document doc;

	auto res = doc.load_file(path.c_str());
	if (!res) {
		write_to_log("%\n", res.description());
		return false;
	}

	pugi::xml_node root = doc.child("animations");
	std::string texture_path = local_folder + root.attribute("image").as_string(),
		texture_id = id_by_path(texture_path);
	const sf::Texture *texture = txs.get_or_load(texture_id, texture_path.c_str());

	auto load_animation_node = [this, texture, &local_folder](const pugi::xml_node &node, Direction dir, 
															  std::unique_ptr<AnimationGroup> &grp) {
		Animation &anim = grp->operator[](dir);
		anim.clear();
		anim.set_duration(sf::milliseconds(node.attribute("duration").as_int()));
		for (auto &cut_node : node.children("cut")) {
			anim.add_frame(sf::IntRect(cut_node.attribute("x").as_int(), cut_node.attribute("y").as_int(),
							 cut_node.attribute("width").as_int(), cut_node.attribute("height").as_int()),
						   texture);
		}
	};

	
	for (auto &group_node : doc.child("animations").children("group")) {
		std::unique_ptr<AnimationGroup> grp(new AnimationGroup);
		std::string name = group_node.attribute("name").as_string();
		load_animation_node(group_node.child("east"), Direction::EAST, grp);
		load_animation_node(group_node.child("north"), Direction::NORTH, grp);
		load_animation_node(group_node.child("northeast"), Direction::NORTHEAST, grp);
		load_animation_node(group_node.child("northwest"), Direction::NORTHWEST, grp);
		load_animation_node(group_node.child("south"), Direction::SOUTH, grp);
		load_animation_node(group_node.child("southeast"), Direction::SOUTHEAST, grp);
		load_animation_node(group_node.child("west"), Direction::WEST, grp);
		load_animation_node(group_node.child("southwest"), Direction::SOUTHWEST, grp);
		this->add(std::move(name), std::move(grp));
	}

	return true;
}
