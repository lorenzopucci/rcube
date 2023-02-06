/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <vector>

#include <rcube.hpp>
#include <utility.hpp>

#include "util.hpp"

std::vector<Color> getAdjacentColors(const Color &color)
{
    switch (color)
    {
        case Color::White:
        case Color::Yellow:
            return {Color::Blue, Color::Red, Color::Green, Color::Orange};

        case Color::Red:
        case Color::Orange:
            return {Color::Blue, Color::White, Color::Green, Color::Yellow};

        case Color::Blue:
        case Color::Green:
            return {Color::White, Color::Red, Color::Yellow, Color::Orange};
    }
}

Color getOppositeColor(const Color &color)
{
    switch (color)
    {
        case Color::White: return Color::Yellow;
        case Color::Yellow: return Color::White;
        case Color::Red: return Color::Orange;
        case Color::Orange: return Color::Red;
        case Color::Blue: return Color::Green;
        case Color::Green: return Color::Blue;
    }
}

rcube::Coordinates getBlockPlace(rcube::Cube *cube, const Color &c1,
    const Color &c2)
{
    return rcube::Coordinates(
        cube->getFaceOrientation(c1),
        cube->getFaceOrientation(c2)
    );
}

rcube::Coordinates getBlockPlace(rcube::Cube *cube, const Color &c1,
    const Color &c2, const Color &c3)
{
    return rcube::Coordinates(
        cube->getFaceOrientation(c1),
        cube->getFaceOrientation(c2),
        cube->getFaceOrientation(c3)
    );
}

rcube::Move pullUp(const rcube::Coordinates &pos)
{
    rcube::Orientation layer = {Axis::Z, pos.z()};
    int direction = layer.getRotated(Axis::X, pos.x()).direction;
    return rcube::Move({Axis::X, pos.x()}, direction);
}

rcube::Move getUmove(rcube::Coordinates pos, const rcube::Orientation &orient)
{
    pos.rotate(Axis::Y, 1);
    bool toInvert = pos.coords[orient.axis] == orient.direction;
    return rcube::Move('U', toInvert ? -1 : 1);
}