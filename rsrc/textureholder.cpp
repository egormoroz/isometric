#include "textureholder.h"
#include "../util/stringhelper.h"

bool TextureHolder::load_file(const char *file_name)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture);
	if (!texture->loadFromFile(file_name)) {
		//sf::Texture::loadFromFile will print the reason of the error
		return false;
	}

	this->add(id_by_path(file_name), std::move(texture));

	return true;
}
