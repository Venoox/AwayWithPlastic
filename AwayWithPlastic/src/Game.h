#pragma once
#include <GL/glew.h>
#include <SDL.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Game {
private:
    bool isRunning;
    std::string title;
    SDL_GLContext context;
    
    int width;
    int height;
public:
    int tile;
    
    SDL_Window* window;

    int init(std::string title, int width, int height, int tilesize);
    inline void SwapBuffer() const { SDL_GL_SwapWindow(window); }
    bool running();
    ~Game();
	void setWindowSize(int width, int height);
    inline int GetWidth() const { return width; }
    inline int GetHeight() const { return height; }
};