#pragma once

#include <map>

#include <rcube.hpp>
#include <utility.hpp>

#include "indexBuffer.hpp"
#include "vertexBuffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "camera.hpp"

static const float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f
};

static const unsigned int indices[] = {
        1, 3, 0, // Left
        3, 0, 2,

        3, 2, 6, // Top
        2, 6, 7,

        0, 4, 2, // Back
        4, 2, 6,

        5, 7, 6, // Right
        7, 6, 4,

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
        Cube(rcube::BlockArray blocks);

        void bindToVertexArray(VertexArray* va);

        void draw(VertexArray* va, Shader* shader, Camera* camera);
private:
        rcube::Block blocks[26];
};