#include "TextureManager.h"

int TextureManager::n = 0;

void TextureManager::add(std::string path)
{
	n++;
	textures.push_back(new Texture(path));
	textures.back()->Bind(n);
}
