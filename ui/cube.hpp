/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <map>

#include <rcube.hpp>
#include <utility.hpp>

#include "indexBuffer.hpp"
#include "vertexBuffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "camera.hpp"

const float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
};

const unsigned int indices[] = {
        1, 3, 0, // Left
        3, 0, 2,

        3, 2, 6, // Top
        3, 6, 7,

        0, 4, 2, // Back
        4, 2, 6,

        5, 7, 6, // Right
        5, 6, 4,

        0, 1, 4, // Bottom
        1, 4, 5,

        1, 3, 5, // Front
        3, 5, 7,
};

struct RGBA
{
    float red;
    float green;
    float blue;
    float alpha = 1.0f;
};

class Cube
{
public:
        Cube(const rcube::BlockArray &blocks);

        void update(const rcube::BlockArray &blocks);

        void draw(Camera* camera, Shader *shader);

private:
        rcube::BlockArray blocks;

        VertexArray *va = new VertexArray();
        VertexBuffer *vb;
};