/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <iostream>
#include <assert.h>

#include <rcube.hpp>
#include <utility.hpp>


rcube::Cube::Cube(const std::map<rcube::Orientation, rcube::MixedFace> &data)
{
    if (data.size() != 6)
    {
        throw std::invalid_argument("Data length must be 6");
    }

    std::vector<rcube::Orientation> ortIt = rcube::Orientation::iterate();
    for (int i = 0; i < 6; ++i)
    {
        centers[i] = rcube::Center(data.at(ortIt[i]).center, ortIt[i]);
    }

    int eIdx = 0, cIdx = 0;
    rcube::Orientation orients[] = {{Axis::Y, 1}, {Axis::Y, -1}};

    for (int k = 0; k < 2; ++k)
    {
    rcube::MixedFace face = data.at(orients[k]);

    for (int i = 0; i < 4; ++i)
    {
        rcube::Orientation otherOrient = face.adjacentFaces[i];
        rcube::Orientation thirdOrient = face.adjacentFaces[(i + 3) % 4];

        edges[eIdx].location = rcube::Coordinates(orients[k], otherOrient);
        edges[eIdx].stickers[0] = {face.stickers[1 + (i * 2)], orients[k]};

        corners[cIdx].location = rcube::Coordinates(orients[k], otherOrient,
            thirdOrient);
        corners[cIdx].stickers[0] = {face.stickers[i * 2], orients[k]};
        
        for (int x = 0; x < 4; ++x)
        {
            if (data.at(thirdOrient).adjacentFaces[x] == orients[k])
            {
                // with that orientation, there are actually 2 corners
                int toAdd = 0;
                if (data.at(thirdOrient).adjacentFaces[(x + 1) % 4] ==
                    otherOrient) toAdd = 2;

                corners[cIdx].stickers[2] = {
                    data.at(thirdOrient).stickers[((x * 2) + toAdd) % 8],
                    thirdOrient};
            }

            if (data.at(otherOrient).adjacentFaces[x] != orients[k]) continue;

            edges[eIdx].stickers[1] = {
                data.at(otherOrient).stickers[1 + (x * 2)], otherOrient};

            // with that orientation, there are actually 2 corners
            int toAdd = 0;
            if (data.at(otherOrient).adjacentFaces[(x + 1) % 4] == thirdOrient)
                toAdd = 2;

            corners[cIdx].stickers[1] = {
                data.at(otherOrient).stickers[((x * 2) + toAdd) % 8],
                otherOrient};
        }
        ++eIdx;
        ++cIdx;
    }
    }

    rcube::Orientation lOrients[] = {{Axis::X, 1}, {Axis::Z, -1},
        {Axis::X, -1}, {Axis::Z, 1}};

    for (int i = 0; i < 4; ++i)
    {
        rcube::Orientation oOrient = lOrients[(i + 1) % 4];
        edges[eIdx].location = rcube::Coordinates(lOrients[i], oOrient);
        
        for (int k = 0; k < 4; ++k)
        {
            if (data.at(lOrients[i]).adjacentFaces[k] == oOrient)
            {
                edges[eIdx].stickers[0] = {
                    data.at(lOrients[i]).stickers[1 + (k * 2)], lOrients[i]};
            }
            if (data.at(oOrient).adjacentFaces[k] == lOrients[i])
            {
                edges[eIdx].stickers[1] = {
                    data.at(oOrient).stickers[1 + (k * 2)], oOrient};
            }
        }
        ++eIdx;
    }

    if (eIdx != 12 || cIdx != 8)
    {
        throw std::invalid_argument("Invalid input");
    }
}