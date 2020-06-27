#include "Game.h"

int Game::init(std::string title, int width, int height, int tilesize) {
	tile = tilesize; 
	this->width = width;
	this->height = height;
	this->title = title;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error initialising SDL: " << SDL_GetError() << std::endl;
		return -1;
	}

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //If we select compatibility profile VAO 0 is bound automatically, if we use core profile we have to bind VAO manually.

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == nullptr) {
		std::cout << "Error initialising SDL_Window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

    SDL_GLContext context = SDL_GL_CreateContext(window);
	if (context == nullptr) {
		std::cout << "Error initialising SDL_GLContext: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	if (SDL_GL_SetSwapInterval(1) != 0) {
		std::cout << "Error setting swap interval" << SDL_GetError() << std::endl;
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	if (glewInit() != GLEW_OK) {
		std::cout << "Error initialising GLEW!" << std::endl;
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

    
    //proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
	//view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    isRunning = true;
	return 0;
}


bool Game::running() {
    return isRunning;
}

Game::~Game() {
	//Free up resources
    SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::setWindowSize(int width, int height)
{
	this->width = width;
	this->height = height;
	//proj = glm::ortho(0.0f, (float)0, 0.0f, (float)0, -1.0f, 1.0f);
}
