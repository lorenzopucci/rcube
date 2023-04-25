/*
* Copyright (c) 2023 Lorenzo Pucci
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
#include "texture.hpp"

#define STICKER_TXT_PATH "ui/res/textures/stickers.bmp"

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

const unsigned int vtxIndices[] = {
        1, 3, 0, 2, // Left
        2, 3, 6, 7, // Top
        0, 2, 4, 6, // Back
        4, 5, 6, 7, // Right
        0, 1, 4, 5, // Bottom
        1, 3, 5, 7  // Front
};

const unsigned int defIdxs[] = {0, 1, 2, 1, 2, 3};

class Cube
{
public:
        Cube(const rcube::BlockArray &blocks);

        void update(const rcube::BlockArray &blocks);

        void draw(Camera* camera, Shader *shader);

private:
        Texture *texture;
        IndexBuffer *ib = new IndexBuffer(defIdxs, 6);

        rcube::BlockArray blocks;
};