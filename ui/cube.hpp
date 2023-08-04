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
#include <algorithm>

#include <rcube.hpp>
#include <utility.hpp>

#include "indexBuffer.hpp"
#include "vertexBuffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "renderer.hpp"

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

extern std::map<char, glm::vec3> colors;

struct Cubie
{
        std::map<rcube::Orientation, Color> stickers;
        
        rcube::Coordinates formalPos;
        rcube::Coordinates finalPos;

        glm::vec3 pos;
        glm::vec3 startPos;
        
        glm::quat orient = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::quat finalOrient = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};

class Cube
{
public:
        Cube(rcube::Cube *cube);

        void performMove(const rcube::Move &move);

        void update();

        void draw(Camera* camera, Shader *shader);

        void updateAnimations();

        rcube::Cube *cube;

private:

        void addAnimation(const rcube::Move &move);

        Texture *texture;
        IndexBuffer *ib = new IndexBuffer(defIdxs, 6);

        Cubie cubies[26];
        double lastCall = 0;
};