#include "renderer.hpp"
#include "vertexArray.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"


void Renderer::draw(VertexArray* va, IndexBuffer* ib, Shader* shader) const
{
    shader->bind();
    va->bind();
    ib->bind();
    glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);
}