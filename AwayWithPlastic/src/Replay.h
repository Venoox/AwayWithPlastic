#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>

struct Replay_Entry {
	int x, y;
};

class Replay
{
private:
	Replay_Entry tmp;
	std::fstream dat;
	std::filesystem::path folder;
public:
	Replay();
	void write(std::string name, int x, int y);

	void read();
	void clear(std::string name);
	void start(std::string name);
	void stop();
	bool isEOF();
	int getX();
	int getY();
};

