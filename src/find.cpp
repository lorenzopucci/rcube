/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <iostream>

#include <rcube.hpp>


rcube::Coordinates rcube::Cube::find(const Color& c1)
{
    for (int i = 0; i < 6; ++i)
    {
        if (centers[i].color == c1) return centers[i].location;
    }
}

rcube::Coordinates rcube::Cube::find(const Color& c1, const Color &c2)
{
    for (int i = 0; i < 12; ++i)
    {
        for (int k = 0; k < 2; ++k)
        {
            if (edges[i].stickers[k].color != c1 &&
                edges[i].stickers[k].color != c2) break;
            
            if (k == 1) return edges[i].location;
        }
    }
    throw std::invalid_argument("Cannot find edge (" + std::to_string((char)c1)
        + ", " +  std::to_string((char)c2) + ")");
}

rcube::Coordinates rcube::Cube::find(const Color& c1, const Color &c2,
    const Color &c3)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int k = 0; k < 3; ++k)
        {
            if (corners[i].stickers[k].color != c1 &&
                corners[i].stickers[k].color != c2 &&
                corners[i].stickers[k].color != c3) break;
            
            if (k == 2) return corners[i].location;
        }
    }
    throw std::invalid_argument("Cannot find corner (" +
        std::to_string((char)c1) + ", " + std::to_string((char)c2) + ", " +
        std::to_string((char)c3) + ")");
}