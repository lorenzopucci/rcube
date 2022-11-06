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

rcube::Orientation rcube::Cube::getStickerOrientation(const rcube::Coordinates
    &coords, const Color &color)
{
    int blockType = abs(coords.x()) + abs(coords.y()) + abs(coords.z());

    if (blockType == 3) // corner
    {
        for (int i = 0; i < 8; ++i)
        {
            if (corners[i].location != coords) continue;

            for (int k = 0; k < 3; ++k)
            {
                if (corners[i].stickers[k].color == color)
                    return corners[i].stickers[k].orientation;
            }

            break;
        }
    }
    else if (blockType == 2) // edge
    {
        for (int i = 0; i < 12; ++i)
        {
            if (edges[i].location != coords) continue;

            for (int k = 0; k < 2; ++k)
            {
                if (edges[i].stickers[k].color == color)
                    return edges[i].stickers[k].orientation;
            }

            break;
        }
    }
    else if (blockType == 1) // center
    {
        for (int i = 0; i < 6; ++i)
        {
            if (centers[i].location == coords && centers[i].color == color)
                return centers[i].orientation;
        }
    }

    throw std::invalid_argument("Cannot find a " + std::to_string((char)color)
        + " sticker at " + coords.to_string());
}

Color rcube::Cube::getFaceColor(const rcube::Orientation &face)
{
    return getCenterFrom(face)->color;
}

Color rcube::Cube::getStickerAt(const rcube::Coordinates &coords,
        const rcube::Orientation &orient)
{
    int blockType = abs(coords.x()) + abs(coords.y()) + abs(coords.z());

    if (blockType == 3) // corner
    {
        for (int i = 0; i < 8; ++i)
        {
            if (corners[i].location != coords) continue;

            for (int k = 0; k < 3; ++k)
            {
                if (corners[i].stickers[k].orientation == orient)
                    return corners[i].stickers[k].color;
            }

            break;
        }
    }
    else if (blockType == 2) // edge
    {
        for (int i = 0; i < 12; ++i)
        {
            if (edges[i].location != coords) continue;

            for (int k = 0; k < 2; ++k)
            {
                if (edges[i].stickers[k].orientation == orient)
                    return edges[i].stickers[k].color;
            }

            break;
        }
    }
    else if (blockType == 1) // center
    {
        return getCenterFrom(orient)->color;
    }

    throw std::invalid_argument("Cannot find a sticker with orientation (" +
        std::to_string((int)orient.axis) + ", " + std::to_string(
        orient.direction) + ") at " + coords.to_string());
}