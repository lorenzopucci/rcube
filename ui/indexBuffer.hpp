/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

class IndexBuffer
{
public:
        IndexBuffer() = default;
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void bind() const;
        void unbind() const;

        inline unsigned int getCount() const { return count; }
        
private:
        unsigned int id;
        unsigned int count;
};
