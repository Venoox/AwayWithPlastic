#include "IndexBuffer.h"
#include <GL/glew.h> 

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : m_Count(count) {

	glGenBuffers(1, &m_RendererID); //Generate buffer (how many buffers, where to store them)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); //Select a buffer (if you have more array buffer[i])
	//Create and initialise buffer's data (what kind of data are we sending in, size of data, pointer to data or NULL, expected usage of data)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_RendererID);

}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); //Select a buffer
}

void IndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}