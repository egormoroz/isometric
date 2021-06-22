#pragma once

#include "resourceholder.h"
#include <SFML/Graphics/Texture.hpp>
#include <string>

class TextureHolder : public ResourceHolder<std::string, sf::Texture, TextureHolder> {
public:
	//load and add texture with automatically generated id
	bool load_file(const char *file_name);
};
