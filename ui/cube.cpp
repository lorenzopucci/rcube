/*
* Copyright (c) 2023 Lorenzo Pucci
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


Cube::Cube(const rcube::BlockArray &blocks)
{
    texture = new Texture(STICKER_TXT_PATH);
    this->update(blocks);
}

void Cube::update(const rcube::BlockArray &blocks)
{
    this->blocks = blocks;
}

float getx0(const Color &color)
{
    float pos;

    switch (color)
    {
        case Color::White: pos = 0.0f; break;
        case Color::Yellow: pos = 1.0f; break;
        case Color::Green: pos = 2.0f; break;
        case Color::Blue: pos = 3.0f; break;
        case Color::Red: pos = 4.0f; break;
        case Color::Orange: pos = 5.0f; break;
    }
    return float(pos/6.0f);
}

float gety0(const rcube::Coordinates &pos, const rcube::Orientation &face)
{
    if (pos.x() * pos.y() * pos.z() == 0)
    {
        if (abs(pos.x() + pos.y() + pos.z()) == 1) // center
        {
            return float(4.0f/6.0f);
        }

        // edge
        if (pos.x() == 0)
        {
            if ((face.axis == Axis::Z && pos.y() == 1) || (face.axis == Axis::Y
                && pos.z() == 1)) return 1.0f/6.0f;
            return 0.0f;
        }
        else if (face.axis == Axis::X && face.direction == -1)
        {
            if (pos.y() == 1) return 1.0f/6.0f;
            else if (pos.z() == 0) return 0.0f;
            else if (pos.z() == 1) return 2.0f/6.0f;
        }
        else if (face.axis == Axis::X && face.direction == 1)
        {
            if (pos.z() == 1) return 1.0f/6.0f;
            else if (pos.y() == 0) return 0.0f;
            else if (pos.y() == -1) return 2.0f/6.0f;
        }
        else if (pos.x() == -1 && face.axis != Axis::X) return 2.0f/6.0f;

        return float(3.0f/6.0f);
    }
    return float(5.0f/6.0f); // corner
}

void Cube::draw(Camera* camera, Shader* shader)
{
    texture->bind();
    shader->bind();
    shader->setUniform1i("textureSampler", 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render all 26 blocks
    for (int block = 0; block < 26; ++block)
    {
        rcube::Coordinates pos = blocks.blocks[block].position;

        camera->resetTransformations();
        camera->arrangeBlock(pos.coords);
        shader->setUniformMat4f("MVP", camera->getMVP());

        for (int face = 0; face < 6; ++face)
        {
            rcube::Orientation o = {(Axis)(face % 3), (face % 2) * 2 - 1};

            if (blocks.blocks[block].stickers.find(o) ==
                blocks.blocks[block].stickers.end()) continue;
            
            float x0 = getx0(blocks.blocks[block].stickers[o]);
            float y0 = gety0(pos, o);
            float x1 = x0 + 1.0f/6.0f;
            float y1 = y0 + 1.0f/6.0f;

            float squareVertices[20] = {
                0.0f, 0.0f, 0.0f, x0, y0,
                0.0f, 0.0f, 0.0f, x1, y0,
                0.0f, 0.0f, 0.0f, x0, y1,
                0.0f, 0.0f, 0.0f, x1, y1
            };

            for (int i = 0; i < 4; ++i)
            {
                int idx = vtxIndices[(face * 4) + i];

                for (int k = 0; k < 3; ++k)
                    squareVertices[(i * 5) + k] = vertices[(idx * 3) + k];
            }

            VertexArray *va = new VertexArray();
            VertexBuffer vb(squareVertices, 20 * sizeof(float));
            VertexBufferLayout layout;
            layout.push<float>(3);
            layout.push<float>(2);
            va->addBuffer(vb, layout);

            render(va, ib, shader);

            delete va;
        }
    }
    glDisable(GL_BLEND);
}