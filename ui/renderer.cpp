#include "renderer.hpp"
#include "vertexArray.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"


void render(VertexArray* va, IndexBuffer* ib, Shader* shader)
{
    shader->bind();
    va->bind();
    ib->bind();
    glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);
}