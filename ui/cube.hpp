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

#ifdef 0

enum Axis {
        X = 0, Y = 1, Z = 2
};

struct Position
{
        Axis axis;
        int direction; // either +1 or -1
};

struct StickerIndices
{
        unsigned int indices[6];
};

enum Stickers
{
        TL = 0, TC = 1, TR = 2,
        CL = 3, CC = 4, CR = 5,
        BL = 6, BC = 7, BR = 8
};


class Cube
{
public:
        Cube(const Position& position);

        void setStickersColors(char* stickerColors);
        void bindToVertexArray(VertexArray* va);

        void draw(VertexArray* va, Shader* shader);
private:
        char *facesColors;
        float vertices[24];
};

#endif