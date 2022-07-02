#ifdef 0

#include <map>
#include <iostream>

#include "cube.hpp"
#include "renderer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"


std::map<char, Color> colors
{
    {'w', {1.0f, 1.0f, 1.0f}},
    {'y', {1.0f, 1.0f, 0.0f}},
    {'g', {0.0f, 1.0f, 0.0f}},
    {'b', {0.0f, 0.0f, 1.0f}},
    {'r', {1.0f, 0.0f, 0.0f}},
    {'o', {1.0f, 0.5f, 0.0f}}
};

Cube::Cube(const Position& position)
{
    // TODO: init vertices
}

void Cube::setStickersColors(char* stickerColors)
{
    this->stickerColors = stickerColors;
}

void Cube::bindToVertexArray(VertexArray* va)
{
    VertexBufferLayout layout;
    layout.push<float>(3);

    VertexBuffer vb(vertices, 8 * 3 * sizeof(float));
    va->addBuffer(vb, layout);
}

void Cube::draw(VertexArray* va, Shader* shader)
{
    shader->bind();

    for (int face = 0; face < 9; ++face)
    {
        shader->setUniform4f("u_color",
            colors[].red,
            colors[].green,
            colors[].blue,
            colors[].alpha);
            
        IndexBuffer ib();

        render(va, &ib, shader);
    }
}

#endif