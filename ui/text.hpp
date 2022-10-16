/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <string>
#include <vector>
#include <ctime>

#include "shader.hpp"
#include "texture.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "vertexArray.hpp"

#define LINE_SPACING 5

struct Font
{
  std::string path;
  int width, height;
  int charWidth, charHeight;
  float hOffset, vOffset;
};

struct String
{
  std::string content;
  int x, y; // position on screen

  unsigned int id;

  int stickTime;
  time_t startTime;

  VertexArray *va;
  VertexBuffer *vb;
  IndexBuffer *ib;

  void computeBuffers(Font *font, int sW, int sH);
};

#define UBUNTU_PATH "ui/res/textures/Ubuntu_font.bmp"

class Text
{
public:
    Text(const int &sW, const int &sH, const Font &font =
      {UBUNTU_PATH, 512, 512, 16, 32, 1.0f/32.0f, 1.0f/16.0f});

    unsigned int addString(const std::string &_data, const int &_x,
      const int &_y, const int &_stickTime = -1);
    // _sticktime is in seconds. -1 disables this feature
    // negative values for _x and _y will cause the string to be printed with
    // offset _x and _y from the OPPOSITE side of the window (right or bottom)

    void removeString(unsigned int _id);

    void editString(unsigned int _id, const std::string &newData);

    static float toGlCoords(int screenCoords, int windowSize);
    // from [0;windowSize] to [-1;1], used for both x and y

    void onResize(const int &_w, const int &_h);

    void draw();

private:
    Font font;
    Texture *texture;
    Shader *shader;

    int width, height;

    std::vector<String> strings;
    unsigned int lastIndex = 0;
};