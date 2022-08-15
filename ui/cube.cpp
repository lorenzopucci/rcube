#include <map>
#include <algorithm>

#include <rcube.hpp>
#include <utility.hpp>

#include "cube.hpp"
#include "renderer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "camera.hpp"


std::map<Color, RGBA> colors
{
    {Color::White, {1.0f, 1.0f, 1.0f}},
    {Color::Yellow, {1.0f, 1.0f, 0.0f}},
    {Color::Green, {0.0f, 1.0f, 0.0f}},
    {Color::Blue, {0.0f, 0.0f, 1.0f}},
    {Color::Red, {1.0f, 0.0f, 0.0f}},
    {Color::Orange, {1.0f, 0.5f, 0.0f}}
};


Cube::Cube(rcube::BlockArray blocks)
{
    std::copy(blocks.blocks, blocks.blocks + 26, this->blocks);
}

void Cube::bindToVertexArray(VertexArray* va)
{
    VertexBufferLayout layout;
    layout.push<float>(3);

    VertexBuffer vb(vertices, 8 * 3 * sizeof(float));
    va->addBuffer(vb, layout);
}

void Cube::draw(VertexArray* va, Shader* shader, Camera* camera)
{
    shader->bind();
    
    for (int block = 0; block < 26; ++block)
    {
        camera->translate(blocks[block].position.coords);
        shader->setUniformMat4f("MVP", camera->getMVP());

        for (int face = 0; face < 6; ++face)
        {
            rcube::Orientation o = {(Axis)(face % 3), (face % 2) * 2 - 1};
            
            shader->setUniform4f("u_color",
                colors[blocks[block].stickers[o]].red,
                colors[blocks[block].stickers[o]].green,
                colors[blocks[block].stickers[o]].blue,
                colors[blocks[block].stickers[o]].alpha);
                
            IndexBuffer* ib = new IndexBuffer(indices + face * 6, 6);

            render(va, ib, shader);
        }
    }
}