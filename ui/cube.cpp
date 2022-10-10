/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

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


Cube::Cube(const rcube::BlockArray &blocks)
{
    this->update(blocks);
}

void Cube::update(const rcube::BlockArray &blocks)
{
    this->blocks = blocks;
}

void Cube::draw(VertexArray* va, Shader* shader, Camera* camera)
{
    // render black inner cube
    shader->bind();
    camera->scale(3.19f);
    shader->setUniformMat4f("MVP", camera->getMVP());
    shader->setUniform4f("u_color", 0.0f, 0.0f, 0.0f, 1.0f);

    IndexBuffer *ib = new IndexBuffer(indices, 36);
    render(va, ib, shader);
    delete ib;

    camera->resetTransformations();

    // render all 26 blocks
    for (int block = 0; block < 26; ++block)
    {
        camera->arrangeBlock(blocks.blocks[block].position.coords);
        shader->setUniformMat4f("MVP", camera->getMVP());

        for (int face = 0; face < 6; ++face)
        {
            rcube::Orientation o = {(Axis)(face % 3), (face % 2) * 2 - 1};

            if (blocks.blocks[block].stickers.find(o) ==
                blocks.blocks[block].stickers.end()) continue;
            
            RGBA col = colors[blocks.blocks[block].stickers[o]];
            shader->setUniform4f("u_color", col.red, col.green, col.blue,
                col.alpha);
                
            IndexBuffer ib(indices + (face * 6), 6);

            render(va, &ib, shader);
        }
    }
}