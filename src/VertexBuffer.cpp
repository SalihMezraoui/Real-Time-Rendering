#include "../include/VertexBuffer.h"

#include "../include/DataBuffer.h"

VertexBuffer::VertexBuffer() {
	glGenVertexArrays(1, &m_vertexBuffer);
}

VertexBuffer::VertexBuffer(GLsizei bufferCount, GLuint* bufferArrays) {
	glGenVertexArrays(bufferCount, bufferArrays);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteVertexArrays(1, &m_vertexBuffer);
}

void VertexBuffer::bind() {
	// Bind the vertex array using the stored buffer ID
	glBindVertexArray(m_vertexBuffer);
}

void VertexBuffer::unbind() {
	// Unbind the vertex array by binding to zero
	glBindVertexArray(0);
}

void VertexBuffer::setAttributePointer(GLuint index, GLint attributeSize, GLenum dataType, GLboolean normalized, GLsizei byteStride, const GLvoid* offset)
{
	// Define an array of vertex attribute data
	glVertexAttribPointer(index, attributeSize, dataType, normalized, byteStride, offset);
}

void VertexBuffer::enableAttribute(GLuint index) {
	// Enable the vertex attribute at the specified index
	glEnableVertexAttribArray(index);
}

void VertexBuffer::attachBuffer(GLuint buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

