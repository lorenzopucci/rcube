#pragma once

#include "vertexArray.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"

class Renderer
{
public:
    void draw(VertexArray* va, IndexBuffer* ib, Shader* shader) const;
};