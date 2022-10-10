/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <vector>
#include <GL/gl.h>

class VertexBuffer
{
public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void bind() const;
        void unbind() const;

private:
        unsigned int id;
};


struct VertexBufferElement
{
        unsigned int count;
        unsigned int type;
        unsigned char normalized;

        static unsigned int getSizeOfType (unsigned int type)
        {
                switch (type)
                {
                        case GL_FLOAT: return 4;
                        case GL_UNSIGNED_INT: return 4;
                        case GL_UNSIGNED_BYTE: return 1;
                }
                return 0;
        }
};


struct VertexBufferLayout
{
public:
        VertexBufferLayout()
                : stride(0) {}

        template<typename T>
        void push(unsigned int count);

        inline unsigned int getStride() const
        {
                return stride;
        }

        inline const std::vector<VertexBufferElement>& getElements() const
        {
                return elements;
        }
        
private:
        std::vector<VertexBufferElement> elements;
        unsigned int stride = 0;
};
