#include <GL/glew.h>

#include "vertexBuffer.hpp"

VertexBuffer::VertexBuffer (const void* data, unsigned int size)
{
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &id);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() const
{
       glBindBuffer(GL_ARRAY_BUFFER, 0);
}


template<>
void VertexBufferLayout::push<float>(unsigned int count)
{
    elements.push_back((VertexBufferElement){count, GL_FLOAT, GL_FALSE});
    stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    elements.push_back((VertexBufferElement){count, GL_UNSIGNED_INT,
        GL_FALSE});
    stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    elements.push_back((VertexBufferElement){count, GL_UNSIGNED_BYTE,
        GL_TRUE});
    stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}

