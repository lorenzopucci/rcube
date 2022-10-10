/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <vector>
#include <assert.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "text.hpp"
#include "texture.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "vertexArray.hpp"
#include "renderer.hpp"

void String::computeBuffers(Font *font, int sW, int sH)
{
    float vBuffer[content.length() * 16];
    unsigned int iBuffer[content.length() * 6];

    int currX = 0, currY = 0;

	for (unsigned int i = 0 ; i < content.length(); ++i)
    {
        if (content[i] == '\n')
        {
            currY += font->charHeight + LINE_SPACING;
            currX = 0;
            continue;
        }

        glm::vec2 vertices[4] = {
            {currX, currY + font->charHeight},
            {currX, currY},
            {currX + font->charWidth, currY + font->charHeight},
            {currX + font->charWidth, currY}
        };

        float charX = float(font->vOffset * (content[i] % 16));
        float charY = float(font->vOffset * (17 - content[i] / 16));

        glm::vec2 UVs[4] = {
            {charX, charY},
            {charX, charY + font->vOffset},
            {charX + font->hOffset, charY},
            {charX + font->hOffset, charY + font->vOffset}
        };
        currX += font->charWidth;

        for (int k = 0; k < 4; ++k)
        {
            vBuffer[(i * 16) + (k * 4)] = Text::toGlCoords(vertices[k].x+x, sW);
            vBuffer[(i * 16) + (k * 4) + 1] = Text::toGlCoords(sH -
                (vertices[k].y + y), sH);
            vBuffer[(i * 16) + (k * 4) + 2] = UVs[k].x;
            vBuffer[(i * 16) + (k * 4) + 3] = UVs[k].y;
        }

        for (unsigned int k = 0; k <= 2; ++k)
        {
            iBuffer[(i * 6) + k] = (i * 4) + k;
        }
        for (unsigned int k = 1; k <= 3; ++k)
        {
            iBuffer[(i * 6) + 2 + k] = (i * 4) + k;
        }
	}

    delete vb, ib, va;
    vb = new VertexBuffer(vBuffer, content.length() * 16 * sizeof(float));
    ib = new IndexBuffer(iBuffer, content.length() * 6);
    vb->bind();

    va = new VertexArray();
    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);

    va->addBuffer(*vb, layout);
}

float Text::toGlCoords(int screenCoords, int windowSize)
{
    // from [0;windowSize] to [-1;1], used for both x and y
    return 2.0f * float(screenCoords) / windowSize - 1;
}

Text::Text(const int &sW, const int &sH, const Font &_font)
{
    font = _font;
    texture = new Texture(font.path);
    shader = new Shader(TXT_VS, TXT_FS);

    width = sW;
    height = sH;
}

unsigned int Text::addString(const std::string &_data, const int &_x,
    const int &_y, const int &_stickTime)
{
    lastIndex++;

    String str = {_data, _x, _y, lastIndex, _stickTime, time(NULL)};
    str.computeBuffers(&font, width, height);
    strings.push_back(str);
    
    return lastIndex;
}

void Text::removeString(unsigned int _id)
{
    for (auto it = strings.begin(); it < strings.end(); ++it)
    {
        if (it->id == _id)
        {
            strings.erase(it);
            return;
        }
    }
}

void Text::editString(unsigned int _id, const std::string &newData)
{
    for (auto it = strings.begin(); it < strings.end(); ++it)
    {
        if (it->id == _id)
        {
            it->content = newData;
            it->computeBuffers(&font, width, height);
            return;
        }
    }
}

void Text::onResize(const int &_w, const int &_h)
{
    for (auto it = strings.begin(); it < strings.end(); ++it)
    {
        it->computeBuffers(&font, _w, _h);
    }
    width = _w;
    height = _h;
}

void Text::draw()
{
    texture->bind();
    shader->bind();
    shader->setUniform1i("textureSampler", 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto it = strings.begin(); it < strings.end(); ++it)
    {
        if (it->stickTime != -1 && time(NULL) - it->startTime >= it->stickTime)
        {
            strings.erase(it);
            continue;
        }

        render(it->va, it->ib, shader);
    }

    glDisable(GL_BLEND);
}

