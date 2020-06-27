#pragma once

#include <SDL.h>
#include <GL/glew.h>

#include <fstream>
#include <string>
#include <sstream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GameObject.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "freetype-gl/freetype-gl.h"


#define ASSERT(x) if (!(x)) std::cin.get();


void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

#define RENDERER_MAX_SPRITES 10000
#define RENDERER_VERTEX_SIZE 16
#define RENDERER_SPRITE_SIZE RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6

struct VertexData {
    float x, y, tex1, tex2;
};

class Renderer {
    private:
    VertexBuffer vb;
    VertexBufferLayout layout;
    VertexArray va;
    IndexBuffer* ib;
    Shader shader;
    VertexData* buffer;
	std::vector<Texture> textures;
    unsigned int IndexCount;
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 mvp;
	ftgl::texture_atlas_t* FTAtlas;
	ftgl::texture_font_t* FTFont;
	Texture* string;
public:
    Renderer(int width, int height);
	void SetWindowsSize(int width, int height);
	void Clear() const;
	void Draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader, const GameObject &player, const Game &game) const;
    void BatchBegin();
    void BatchDraw(const GameObject &player);
	void BatchDrawString(int x, int y, std::string text);
    void BatchEnd();
    void Flush(int tex = 3);
};