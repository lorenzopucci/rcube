#include <GL/glew.h>

#include "indexBuffer.hpp"

IndexBuffer::IndexBuffer (const unsigned int* data, unsigned int count)
        : count(count)
{
        unsigned int size = count * sizeof(unsigned int);
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &id);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind() const
{
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

