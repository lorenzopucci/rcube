#pragma once

#include <string>
#include <vector>

#include "shader.hpp"
#include "texture.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "vertexArray.hpp"

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
      const int &_y);

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