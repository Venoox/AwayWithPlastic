#include "Highscore.h"

void Highscore::remove(std::string name)
{
	for (auto it = list.begin(); it != list.end(); ) {
		if ((*it).name == name) {
			it = list.erase(it);
		}
		else {
			it++;
		}
	}
}

void Highscore::add(std::string name, int points)
{
	remove(name);
	strcpy_s(tmp.name, 19, name.c_str());
	tmp.score = points;
	if (!list.empty()) {
		if (list.front().score <= tmp.score) {
			list.push_front(tmp);
		}
		else if (list.back().score > tmp.score) {
			list.push_back(tmp);
		}
		else {
			auto it = list.begin();
			for (; it != list.end(); it++) {
				if ((*it).score <= tmp.score) {
					break;
				}
			}
			list.insert(it, tmp);
		}
	}
	else {
		list.push_back(tmp);
	}
}

void Highscore::write()
{
	dat.open("highscore.bin", std::ios::binary | std::ios::out | std::ios::trunc);
	if (dat.is_open()) {
		for (auto it = list.begin(); it != list.end(); it++) {
			tmp = *it;
			dat.write((char*)& tmp, sizeof(tmp));
		}
		dat.close();
	}
}

void Highscore::read()
{
	dat.open("highscore.bin", std::ios::binary | std::ios::in);
	if (dat.is_open()) {
		while (dat.read((char*)& tmp, sizeof(tmp))) {
			list.push_back(tmp);
		}
		dat.close();
	}
}

void Highscore::print()
{
	for (auto it = list.begin(); it != list.end(); it++) {
		std::cout << (*it).name << " " << (*it).score << std::endl;
	}
}
