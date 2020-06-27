#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include "GameObject.h"

struct GameState {
	int points, difficulty;
};

class Save {
private:
	std::fstream dat;
	std::filesystem::path folder;
	GameState tmp;
public:
	bool loadAll(std::string name, std::list<GameObject*>& enemy, std::list<GameObject*>& trash, std::list<GameObject*>& teammate, GameObject*& player, GameObject*& ship, int& points, int& diff);
	void saveAll(std::string name, std::list<GameObject*>& enemy, std::list<GameObject*>& trash, std::list<GameObject*>& teammate, GameObject*& player, GameObject*& ship, int& points, int& diff);
};