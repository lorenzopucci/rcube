#pragma once

#include "indexBuffer.hpp"
#include "vertexBuffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"

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
        0, 1, 4,
        1, 4, 5,

        0, 4, 2,
        4, 2, 6,

        1, 3, 5,
        3, 5, 7,

        5, 7, 6,
        7, 6, 4,

        1, 3, 0,
        3, 0, 2,

        3, 2, 6,
        2, 6, 7
};

struct Color
{
    float red;
    float green;
    float blue;
    float alpha = 1.0f;
};