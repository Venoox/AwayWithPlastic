#include "Save.h"


bool Save::loadAll(std::string name, std::list<GameObject*>& enemy, std::list<GameObject*>& trash, std::list<GameObject*>& teammate, GameObject*& player, GameObject*& ship, int& points, int& diff)
{
	folder = "save\\" + name + "\\";
	if (std::filesystem::exists(folder)) {
		dat.open(folder.string() + "\\enemy.bin", std::ios::binary | std::ios::in);
		GameObject tmp;
		if (dat.is_open()) {
			while (dat.read((char*)& tmp, sizeof(GameObject))) {
				enemy.push_back(new GameObject(tmp));
			}
			dat.close();
		}
		dat.open(folder.string() + "\\teammate.bin", std::ios::binary | std::ios::in);
		if (dat.is_open()) {
			while (dat.read((char*)& tmp, sizeof(GameObject))) {
				teammate.push_back(new GameObject(tmp));
			}
			dat.close();
		}
		dat.open(folder.string() + "\\trash.bin", std::ios::binary | std::ios::in);
		if (dat.is_open()) {
			while (dat.read((char*)& tmp, sizeof(GameObject))) {
				trash.push_back(new GameObject(tmp));
			}
			dat.close();
		}
		dat.open(folder.string() + "\\player.bin", std::ios::binary | std::ios::in);
		if (dat.is_open()) {
			dat.read((char*)& tmp, sizeof(GameObject));
			player = new GameObject(tmp);
			dat.close();
		}
		dat.open(folder.string() + "\\ship.bin", std::ios::binary | std::ios::in);
		if (dat.is_open()) {
			dat.read((char*)& tmp, sizeof(GameObject));
			ship = new GameObject(tmp);
			dat.close();
		}
		dat.open(folder.string() + "\\gamestate.bin", std::ios::binary | std::ios::in);
		if (dat.is_open()) {
			dat.read((char*)& this->tmp, sizeof(GameState));
			diff = this->tmp.difficulty;
			points = this->tmp.points;
			dat.close();
		}
		return true;
	}
	else {
		std::cerr << "Folder doesn't exist!" << std::endl;
		return false;
	}
}

void Save::saveAll(std::string name, std::list<GameObject*>& enemy, std::list<GameObject*>& trash, std::list<GameObject*>& teammate, GameObject*& player, GameObject*& ship, int& points, int& diff)
{
	folder = "save\\" + name + "\\";
	if (!std::filesystem::exists(folder)) {
		if (!std::filesystem::create_directories(folder)) {
			std::cerr << "Error creating folder!" << std::endl;
		}
	}
	if (std::filesystem::exists(folder)) {
		dat.open(folder.string() + "\\enemy.bin", std::ios::binary | std::ios::out);
		if (dat.is_open()) {
			for (auto it = enemy.begin(); it != enemy.end(); it++) {
				dat.write((char*) & (**it), sizeof(GameObject));
			}
			dat.close();
		}
		dat.open(folder.string() + "\\teammate.bin", std::ios::binary | std::ios::out);
		if (dat.is_open()) {
			for (auto it = teammate.begin(); it != teammate.end(); it++) {
				dat.write((char*) & (**it), sizeof(GameObject));
			}
			dat.close();
		}
		dat.open(folder.string() + "\\trash.bin", std::ios::binary | std::ios::out);
		if (dat.is_open()) {
			for (auto it = trash.begin(); it != trash.end(); it++) {
				dat.write((char*) & (**it), sizeof(GameObject));
			}
			dat.close();
		}
		dat.open(folder.string() + "\\player.bin", std::ios::binary | std::ios::out);
		if (dat.is_open()) {
			dat.write((char*) player, sizeof(GameObject));
			dat.close();
		}
		dat.open(folder.string() + "\\ship.bin", std::ios::binary | std::ios::out);
		if (dat.is_open()) {
			dat.write((char*) ship, sizeof(GameObject));
			dat.close();
		}
		dat.open(folder.string() + "\\gamestate.bin", std::ios::binary | std::ios::out);
		tmp.difficulty = diff;
		tmp.points = points;
		if (dat.is_open()) {
			dat.write((char*)& tmp, sizeof(GameState));
			dat.close();
		}
	}
}
