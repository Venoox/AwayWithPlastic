#include "VertexBuffer.h"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void *data, unsigned int size, unsigned int usage) {
	glGenBuffers(1, &m_RendererID); //Generate buffer (how many buffers, where to store them)
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); //Select a buffer (if you have more array buffer[i])
	//Create and initialise buffer's data (what kind of data are we sending in, size of data, pointer to data or NULL, expected usage of data)
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_RendererID);

}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); //Select a buffer
}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int VertexBuffer::getID() {
	return m_RendererID;
}