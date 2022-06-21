#include <vector>

#include <GL/glew.h>

#include "vertexArray.hpp"
#include "vertexBuffer.hpp"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &id);
}

void VertexArray::addBuffer (
        const VertexBuffer& vb, 
        const VertexBufferLayout& layout)
{
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); ++i) {
        const VertexBufferElement& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,
            element.count,
            element.type,
            element.normalized,
            layout.getStride(),
            (const void*)offset);
        offset += element.count *
            VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const
{
    glBindVertexArray(id);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}
