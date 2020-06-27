#include "Replay.h"

Replay::Replay()
{
	
}

void Replay::write(std::string name, int x, int y)
{
	folder = "save\\" + name + "\\";
	if (!std::filesystem::exists(folder)) {
		if (!std::filesystem::create_directories(folder)) {
			std::cerr << "Error creating folder!" << std::endl;
		}
	}
	if (std::filesystem::exists(folder)) {
		dat.open(folder.string() + "replay.bin", std::ios::binary | std::ios::out | std::ios::app);
		if (dat.is_open()) {
			tmp.x = x;
			tmp.y = y;
			dat.write((char*)& tmp, sizeof(tmp));
			dat.close();
		}
	}
}

void Replay::clear(std::string name)
{
	folder = "save\\" + name + "\\";
	if (std::filesystem::exists(folder)) {
		dat.open(folder.string() + "replay.bin", std::ios::binary | std::ios::out | std::ios::trunc);
		if (dat.is_open()) {
			dat.close();
		}
	}
}

void Replay::start(std::string name)
{
	folder = "save\\" + name + "\\";
	if (!std::filesystem::exists(folder)) {
		if (!std::filesystem::create_directories(folder)) {
			std::cerr << "Error creating folder!" << std::endl;
		}
	}
	if (std::filesystem::exists(folder)) {
		dat.open(folder.string() + "replay.bin", std::ios::binary | std::ios::in);
	}
}

void Replay::read()
{
	if (dat.is_open()) {
		dat.read((char*)& tmp, sizeof(tmp));
	}
}

void Replay::stop()
{
	if (dat.is_open()) {
		dat.close();
	}
}

bool Replay::isEOF() {
	if (dat.is_open()) {
		if (dat.eof()) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
}

int Replay::getX()
{
	return tmp.x;
}

int Replay::getY()
{
	return tmp.y;
}
