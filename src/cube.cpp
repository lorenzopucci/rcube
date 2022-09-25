#include <map>
#include <set>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <assert.h>

#include <rcube.hpp>
#include <utility.hpp>

using pairCoords2DColor = std::pair<rcube::Coordinates2D, Color>;
using pairOrientColor = std::pair<rcube::Orientation, Color>;


rcube::Center* rcube::Cube::getCenterFrom(const Color &color)
{
    for (int i = 0; i < 6; ++i)
    {
        if (centers[i].color == color) return centers + i;
    }
    assert(false);
}

rcube::Center* rcube::Cube::getCenterFrom(const rcube::Coordinates &coords)
{
    for (int i = 0; i < 6; ++i)
    {
        if (centers[i].location == coords) return centers + i;
    }
    assert(false);
}

std::string colorize (const char& color)
{
    switch (color)
    {
        case 'y': return "\033[103;30mY\033[0m";
        case 'w': return "\033[47;30mW\033[0m";
        case 'g': return "\033[42;30mG\033[0m";
        case 'o': return "\033[43;30mO\033[0m";
        case 'b': return "\033[44;37mB\033[0m";
        case 'r': return "\033[41;37mR\033[0m";
        default: return "\033[0m" + color;
    }
}

void rcube::Cube::display ()
{
    rcube::Net toDisplay = netRender();
    for (rcube::Orientation o : rcube::Orientation::iterate())
    {
        std::cout << "\nFace at orientation (" << o.axis << "; " <<
            o.direction << "):" << std::endl;

        for (int y = 1; y >= -1; --y)
        {
            for (int x = -1; x <= 1; ++x)
            {
                std::cout << colorize((char)toDisplay.faces[o].stickers[
                    {x, y}]);
            }
            std::cout << std::endl;
        }
    }
}

rcube::Cube::Cube(const Color& topColor, const Color& frontColor)
{
    // centers, edges and corners are first initialized in the default position
    // (top: white, front: green), then the cube is rotated until the desired
    // position is reached

    std::vector<rcube::Orientation> orients = rcube::Orientation::iterate();
    char defaultColors[] = {'o', 'r', 'y', 'w', 'b', 'g'};

    for (int i = 0; i < 6; ++i)
    {
        centers[i] = rcube::Center((Color)defaultColors[i], orients[i]);
    }

    // initialize edges and corners
    int eIdx = 0; // index in edges array
    int cIdx = 0; // index in corner array
    rcube::Orientation o[3];

    for (int a1 = 0, d1 = -1; a1 < 2; d1 *= -1)
    {
        o[0] = {(Axis)a1, d1};
        o[2] = {(Axis)((a1 + 1) % 2), d1 * (-1 + a1 * 2)};

        for (int d2 = -1; d2 < 2; d2 += 2)
        {
            o[1] = {Axis::Z, d2};

            edges[eIdx].location = rcube::Coordinates(o[0], o[1]);
            for (int i = 0; i <= 1; ++i)
            {
                edges[eIdx].stickers[i] = {
                    getCenterFrom(rcube::Coordinates(o[i]))->color, o[i]};
            }

            corners[cIdx].location = rcube::Coordinates(o[0], o[1], o[2]);
            for (int i = 0; i <= 1; ++i)
                corners[cIdx].stickers[i] = edges[eIdx].stickers[i];
            corners[cIdx].stickers[2] = {
                getCenterFrom(rcube::Coordinates(o[2]))->color, o[2]};
            cIdx++;
            eIdx++;
        }

        edges[eIdx].location = rcube::Coordinates(o[0], o[2]);
        for (int i = 0; i <= 1; ++i)
        {
            edges[eIdx].stickers[i] = {
                getCenterFrom(rcube::Coordinates(o[i*2]))->color, o[i*2]};
        }
        eIdx++;

        if (d1 == 1) a1++;
    }

    // rotate the cube to the desired position
    if (getCenterFrom(topColor)->location.coords[Axis::X] != 0)
        this->performMove(rcube::Move(MoveFace::ROTATE_Z, MoveDirection::CW));

    for (int i = 0; i < 4; ++i)
    {
        if (getCenterFrom(rcube::Coordinates(0, 1, 0))->color == topColor)
            break;
        this->performMove(rcube::Move(MoveFace::ROTATE_X, MoveDirection::CW));
    }

    for (int i = 0; i < 4; ++i)
    {
        if (getCenterFrom(rcube::Coordinates(0, 0, 1))->color == frontColor)
            break;
        this->performMove(rcube::Move(MoveFace::ROTATE_Y, MoveDirection::CW));
    }

    assert(getCenterFrom(rcube::Coordinates(0, 0, 1))->color == frontColor
        && getCenterFrom(rcube::Coordinates(0, 1, 0))->color == topColor);
}

void rcube::Cube::performMove (const rcube::Move& move)
{
    switch (move.face)
    {
        case UP:
        case FRONT:
        case RIGHT:
        case LEFT:
        case DOWN:
        case BACK:
            rotateLayer(move); return;

        case ROTATE_X:
        case ROTATE_Y:
        case ROTATE_Z:
            changeViewpoint(move); return;

        case MIDDLE:
        case EQUATOR:
        case SIDE:
        {
            std::map<MoveFace, std::string> algos = {
                {MIDDLE, "RL'x'"},
                {EQUATOR, "UD'y'"},
                {SIDE, "F'Bz"}
            };

            rcube::Algorithm algo(algos[move.face]);
            performAlgorithm(algo * move.direction);
            return;
        }
    }
}

void rcube::Cube::changeViewpoint (const rcube::Move& move)
{
    for (int i = 0; i < 12; ++i)
    {
        edges[i].location.rotate(move.axis, move.direction);
        for (int k = 0; k < 2; ++k)
        {
            edges[i].stickers[k].orientation.rotate(move.axis, move.direction);
        }

        if (i >= 8) continue;

        corners[i].location.rotate(move.axis, move.direction);
        for (int k = 0; k < 3; ++k)
        {
            corners[i].stickers[k].orientation.rotate(move.axis, move.direction);
        }

        if (i >= 6) continue;
        centers[i].location.rotate(move.axis, move.direction);
        centers[i].orientation.rotate(move.axis, move.direction);
    }
}

void rcube::Cube::rotateLayer(const rcube::Move& move)
{
    rcube::Orientation o = move.getAffectedFace();
    int step = o.direction * move.direction;

    for (int i = 0; i < 12; ++i)
    {
        if (edges[i].location.coords[o.axis] == o.direction)
        {
            edges[i].location.rotate(o.axis, step);
            for (int k = 0; k < 2; ++k)
            {
                if (edges[i].stickers[k].orientation == o) continue;

                edges[i].stickers[k].orientation.rotate(o.axis, step);
            }
        }

        if (i >=8) continue;

        if (corners[i].location.coords[o.axis] == o.direction)
        {
            corners[i].location.rotate(o.axis, step);
            for (int k = 0; k < 3; ++k)
            {
                if (corners[i].stickers[k].orientation == o) continue; 

                corners[i].stickers[k].orientation.rotate(o.axis, step);
            }
        }
    }
}

void rcube::Cube::performAlgorithm (const rcube::Algorithm& algorithm)
{
    for (rcube::Move move : algorithm.algorithm)
        performMove(move);
}

void rcube::Cube::scramble(const int &length, rcube::Algorithm* dest)
{
    rcube::Algorithm scramble = rcube::Algorithm::generateScramble(length);
    this->performAlgorithm(scramble);
    *dest = scramble;
}

rcube::Net rcube::Cube::netRender()
{
    rcube::Net net; // to return

    for (int i = 0; i < 6; ++i)
    {
        rcube::Orientation orient = centers[i].orientation;

        // initialize face
        net.faces.insert(std::pair<rcube::Orientation, rcube::Face>(
            orient,
            (rcube::Face){}
        ));

        // insert center
        net.faces[orient].stickers.insert(pairCoords2DColor(
            rcube::Coordinates2D(centers[i].location, orient),
            centers[i].color
        ));
    }

    for (int i = 0; i < 12; ++i)
    {
        // edges
        rcube::Coordinates coords3D = edges[i].location;

        for (int k = 0; k < 2; ++k) // iterate through stickers
        {
            rcube::Orientation orient = edges[i].stickers[k].orientation;

            net.faces[orient].stickers.insert(pairCoords2DColor(
                rcube::Coordinates2D(coords3D, orient),
                edges[i].stickers[k].color
            ));
        }

        if (i >= 8) continue;

        // corners
        coords3D = corners[i].location;

        for (int k = 0; k < 3; ++k) // iterate through stickers
        {
            rcube::Orientation orient = corners[i].stickers[k].orientation;

            net.faces[orient].stickers.insert(pairCoords2DColor(
                rcube::Coordinates2D(coords3D, orient),
                corners[i].stickers[k].color
            ));
        }
    }

    return net;
}

rcube::BlockArray rcube::Cube::blockRender()
{
    rcube::BlockArray pattern; // to return
    
    int patternIndex = 0;

    for (int i = 0; i < 6; ++i)
    {
        rcube::Orientation orient = centers[i].orientation;
        pattern.blocks[patternIndex] = {rcube::Coordinates(orient), {}};
        pattern.blocks[patternIndex].stickers.insert(pairOrientColor(
            orient,
            centers[i].color
        ));
        patternIndex++;
    }

    for (int i = 0; i < 12; ++i)
    {
        // edges
        pattern.blocks[patternIndex] = rcube::Block {edges[i].location, {}};
        for (int k = 0; k < 2; ++k)
        {
            pattern.blocks[patternIndex].stickers.insert(pairOrientColor(
                edges[i].stickers[k].orientation,
                edges[i].stickers[k].color
            ));
        }
        patternIndex++;

        if (i >= 8) continue;

        // corners
        pattern.blocks[patternIndex] = rcube::Block {corners[i].location, {}};
        for (int k = 0; k < 3; ++k)
        {
            pattern.blocks[patternIndex].stickers.insert(pairOrientColor(
                corners[i].stickers[k].orientation,
                corners[i].stickers[k].color
            ));
        }
        patternIndex++;
    }
    assert(patternIndex == 26);

    return pattern;
}