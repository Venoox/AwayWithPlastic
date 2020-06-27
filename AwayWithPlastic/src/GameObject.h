#pragma once

#include <iostream>
#include "Game.h"

class GameObject {
    private:
    int x;
    int y;
    int width;
    int height;
    int tex;
	int game_width;
	int game_height;

    public:
	bool locked;
	bool locked2;
	bool hidden;
	int tox;
	int toy;

	GameObject();
	GameObject(const GameObject &copy);
    GameObject(Game& game);
    GameObject(Game& game, int texture);
    GameObject(Game& game, int texture, int x, int y);
    GameObject(Game& game, int texture, int x, int y, int width, int height);

    void Move(int x, int y);
    void SetCoord(int x, int y);
    void SetTex(unsigned int slot);

    bool Collision(const GameObject &b);
    double CheckDistance(const GameObject &b);

    inline int GetX() const { return x; }
    inline int GetY() const { return y; }
    inline int GetWidth() const { return width; }
    inline int GetHeight() const { return height; }
    inline int GetTex() const { return tex; }
};
