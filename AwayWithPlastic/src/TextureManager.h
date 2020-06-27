#pragma once
#include <list>

#include "Texture.h"

class TextureManager {
	std::list<Texture*> textures;
	
	static int n;
public:
	inline int size() { return n; }
	void add(std::string path);
};