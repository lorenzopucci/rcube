/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include "vertexBuffer.hpp"

class VertexArray
{
public:
        VertexArray();
        ~VertexArray();
        void addBuffer(
                const VertexBuffer& vb, 
                const VertexBufferLayout& layout);
        void bind() const;
        void unbind() const;
private:
        unsigned int id;
};
