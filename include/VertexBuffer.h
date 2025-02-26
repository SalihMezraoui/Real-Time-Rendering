#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <GL/glew.h>
#include "DataBuffer.h"

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();
	void bind();
	void unbind();
	void enableAttribute(GLuint index);
	VertexBuffer(GLsizei bufferCount, GLuint* bufferArrays);
	void attachBuffer(GLuint bufferID);
	void setAttributePointer(GLuint index, GLint attributeSize, GLenum dataType, GLboolean shouldNormalize, GLsizei byteStride, const GLvoid* offset);

private:
	GLuint m_vertexBuffer;
};

#endif 