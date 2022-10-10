/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

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