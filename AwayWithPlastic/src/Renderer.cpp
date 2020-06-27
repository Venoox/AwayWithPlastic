#include "Renderer.h"
#include <algorithm>
#include <iterator>

void GLClearError() {
	while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line) {
	while(GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] ("  << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT);
}

/* Deprecated */
void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader, const GameObject &player, const Game &game) const {
	shader.Bind();
	va.Bind();
	ib.Bind();
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(player.GetX(), player.GetY(), 0));
	//glm::mat4 mvp = game.proj * game.view * model;
	shader.SetUniformMat4f("u_MVP", mvp);
	//std::cout << player.GetTex() << std::endl;
	shader.SetUniform1i("u_Texture", player.GetTex());
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}


Renderer::Renderer(int width, int height)
	: vb(nullptr, RENDERER_BUFFER_SIZE, GL_DYNAMIC_DRAW), shader("AwayWithPlastic/res/shaders/Basic.shader") {

	layout.Push<float>(2); //position
	layout.Push<float>(2); //texture coordinates
	va.AddBuffer(vb, layout);

	unsigned int indices[RENDERER_INDICES_SIZE];
	int offset = 0;
	for(int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
	{
		indices[i] = offset + 0;
		indices[i+1] = offset + 1;
		indices[i+2] = offset + 2;

		indices[i+3] = offset + 2;
		indices[i+4] = offset + 3;
		indices[i+5] = offset + 0;

		offset += 4;
	}

	ib = new IndexBuffer(indices, RENDERER_INDICES_SIZE);
	IndexCount = 0;
	va.Unbind();

	//proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
	proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	mvp = proj * view * model;
	
	FTAtlas = ftgl::texture_atlas_new(512, 512, 1);
	FTFont = ftgl::texture_font_new_from_file(FTAtlas, 50, "AwayWithPlastic/res/fonts/arial.ttf");
	string = new Texture(FTAtlas->width, FTAtlas->height, FTAtlas->data);
	string->Bind(10);
}

void Renderer::SetWindowsSize(int width, int height)
{
	proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
}

void Renderer::BatchBegin() {
	vb.Bind();
	buffer = (VertexData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void Renderer::BatchDraw(const GameObject &player) {
	buffer->x = (float)player.GetX();
	buffer->y = (float)player.GetY();
	buffer->tex1 = 0.0f;
	buffer->tex2 = 0.0f;
	buffer++;

	buffer->x = (float)player.GetX() + (float)player.GetWidth();
	buffer->y = (float)player.GetY();
	buffer->tex1 = 1.0f;
	buffer->tex2 = 0.0f;
	buffer++;

	buffer->x = (float)player.GetX() + (float)player.GetWidth();
	buffer->y = (float)player.GetY() + (float)player.GetHeight();
	buffer->tex1 = 1.0f;
	buffer->tex2 = 1.0f;
	buffer++;

	buffer->x = (float)player.GetX();
	buffer->y = (float)player.GetY() + (float)player.GetHeight();
	buffer->tex1 = 0.0f;
	buffer->tex2 = 1.0f;
	buffer++;

	IndexCount += 6;
}

void Renderer::BatchDrawString(int pos_x, int pos_y, std::string string)
{
	using namespace ftgl;
	const char* text = string.c_str();
	float x = pos_x;
	for (int i = 0; i < strlen(text); i++)
	{
		texture_glyph_t* glyph = texture_font_get_glyph(FTFont, text + i);
		if (glyph != NULL)
		{
			float kerning = 0.0f;
			if (i > 0)
			{
				kerning = texture_glyph_get_kerning(glyph, text + i - 1);
			}
			x += kerning;
			float x0 = (float)(x + glyph->offset_x);
			float y0 = (float)(pos_y + glyph->offset_y);
			float x1 = (float)(x0 + glyph->width);
			float y1 = (float)(y0 - glyph->height);
			float s0 = glyph->s0;
			float t0 = glyph->t0;
			float s1 = glyph->s1;
			float t1 = glyph->t1;

			buffer->x = x0;
			buffer->y = y0;
			buffer->tex1 = s0;
			buffer->tex2 = t0;
			buffer++;

			buffer->x = x0;
			buffer->y = y1;
			buffer->tex1 = s0;
			buffer->tex2 = t1;
			buffer++;

			buffer->x = x1;
			buffer->y = y1;
			buffer->tex1 = s1;
			buffer->tex2 = t1;
			buffer++;

			buffer->x = x1;
			buffer->y = y0;
			buffer->tex1 = s1;
			buffer->tex2 = t0;
			buffer++;

			IndexCount += 6;

			x += glyph->advance_x;
		}
	}
	this->string->SetData(FTAtlas->data);
}

void Renderer::BatchEnd() {
	glUnmapBuffer(GL_ARRAY_BUFFER);
	vb.Unbind();
}

void Renderer::Flush(int tex) {
	shader.Bind();
	va.Bind();
	ib->Bind();

	shader.SetUniformMat4f("u_MVP", mvp);
	shader.SetUniform1i("u_Texture", tex);

	glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);

	IndexCount = 0;
	ib->Unbind();
	va.Unbind();
	shader.Unbind();
}