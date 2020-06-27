#pragma once

#include <iostream>
#include <fstream>
#include <list>

struct Highscore_Entry {
	char name[20];
	int score;
};

class Highscore {
private:
	Highscore_Entry tmp;
	std::fstream dat;
	void remove(std::string name);
public:
	std::list<Highscore_Entry> list;
	void add(std::string name, int points);
	void write();
	void read();
	void print();
};