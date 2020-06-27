#include "VertexArray.h"

#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
	(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
	Bind();
	vb.Bind();
	const auto &elements = layout.GetElements();
	unsigned int offset = 0;
	for(unsigned int i=0; i<elements.size();i++) {
		const auto &element = elements[i];

		glEnableVertexAttribArray(i); //Enable vertex attribute array (can do it before or after glVertexAttribPointer)
	
		//Define an array of vertex attribute data (index of VAO, number of components, type of data, normalized(GL_FALSE if data is between -1.0 and 1.0), amount of bytes between each vertex, offset between start of the vertex and beggining of attribute)
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset); //Binds currently bound buffer with VAO

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(0);
}