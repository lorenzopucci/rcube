/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <rcube.hpp>

bool handleLetters(Color color, char given, char otherCase,
    std::map<char, Color> *letterMapping)
{
    if (letterMapping->find(given) != letterMapping->end())
    {
        return color == letterMapping->at(given);
    }
    else if (letterMapping->find(otherCase) != letterMapping->end())
    {
        Color newColor;

        switch (letterMapping->at(otherCase))
        {
            case Color::White: newColor = Color::Yellow; break;
            case Color::Yellow: newColor = Color::White; break;
            case Color::Green: newColor = Color::Blue; break;
            case Color::Blue: newColor = Color::Green; break;
            case Color::Red: newColor = Color::Orange; break;
            case Color::Orange: newColor = Color::Red; break;
        }

        letterMapping->insert(std::pair<char, Color>(given, newColor));
        return color == newColor;
    }
    else
    {
        // make sure no other letter is pointing to that color
        for (auto it : *letterMapping)
        {
            if (it.second == color) return false;
        }

        letterMapping->insert(std::pair<char, Color>(given, color));
        return true;
    }
}

bool stickerMatches(const Color &color, const char &match,
    std::map<char, Color> *letterMapping)
{
    switch (match)
    {
        case '*':
            return true;

        case 'W':
        case 'Y':
        case 'G':
        case 'B':
        case 'R':
        case 'O':
            return color == static_cast<Color>(tolower(match));

        case 'w':
        case 'y':
        case 'g':
        case 'b':
        case 'r':
        case 'o':
            return color != static_cast<Color>(match);

        case 'A':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'P':
        case 'Q':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'X':
        case 'Z':
            return handleLetters(color, match, tolower(match), letterMapping);
        
        case 'a':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'p':
        case 'q':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'x':
        case 'z':
            return handleLetters(color, match, toupper(match), letterMapping);
        
        default: return false;
    }
}

char getCharAtCoords(const std::string &pattern, const rcube::Coordinates2D
    &coords)
{
    return pattern[((abs(coords.coords[Axis::Y] - 1)) * 3) +
        coords.coords[Axis::X] + 1];
}

bool rcube::Cube::faceMatches(const rcube::Orientation &face,
    const std::string &expr)
{
    // check the syntax
    if (expr.size() != 9) return false;

    std::map<char, Color> letterMapping;

    // check the center
    if (!stickerMatches(getCenterFrom(face)->color, expr[4], &letterMapping))
        return false;


    for (int rot = 0; rot < 4; ++rot)
    {
        letterMapping.clear();
        // if the center is a letter, it must be reincluded in the map
        stickerMatches(getCenterFrom(face)->color, expr[4], &letterMapping);


        for (int i = 0; i < 12; ++i)
        {
            for (int k = 0; k < 2; ++k)
            {
                if (edges[i].stickers[k].orientation != face) continue;

                rcube::Coordinates2D patternCoords(edges[i].location, face);

                patternCoords.rotate(rot);

                if (!stickerMatches(edges[i].stickers[k].color,
                    getCharAtCoords(expr, patternCoords), &letterMapping))
                        goto nextIt;

            }

            if (i >= 8) continue;

            if (corners[i].location.coords[face.axis] != face.direction)
                continue;

            for (int k = 0; k < 3; ++k)
            {
                if (corners[i].stickers[k].orientation != face) continue;

                rcube::Coordinates2D patternCoords(corners[i].location, face);

                patternCoords.rotate(rot);

                if (!stickerMatches(corners[i].stickers[k].color,
                    getCharAtCoords(expr, patternCoords), &letterMapping))
                        goto nextIt;
            }
        }
        return true;
nextIt:
    ;
    }
    return false;
}

struct MatchingPath
{
    // A MatchingPath is one of the ways in which a string can match a layer.
    // It is determined by a starting sticker (coordinates + orientation) and
    // by the face around which the matching algorithm is performed (layer)

    rcube::Coordinates _startBlock = rcube::Coordinates(0,0,0);
    rcube::Orientation _layer;
    rcube::Orientation _facesOrder[4];

    std::map<char, Color> _letterMapping; // gets fed to stickerMatches()

    MatchingPath(const rcube::Coordinates &startBlock, const rcube::Orientation
        &startStk, const rcube::Orientation &layer,
        const std::map<char, Color> &lm);

    int getBlockNum(const rcube::Coordinates &blockPos,
        const rcube::Orientation &stk);
    // get the distance of a block from the starting point
};

MatchingPath::MatchingPath(const rcube::Coordinates &startBlock, const
    rcube::Orientation &startStk, const rcube::Orientation &layer,
        const std::map<char, Color> &lm)
{
    _startBlock = startBlock;
    _layer = layer;
    _letterMapping = lm;

    int rotStep = 1;
    if ((_layer.axis + 1) % 3 == startStk.axis)
    {
        rotStep = -1;
    }

    int thirdAxis = (3 - (_layer.axis + startStk.axis));
    int rotFactor = startBlock.coords[thirdAxis];

    _facesOrder[0] = startStk;
    _facesOrder[1] = startStk.getRotated(_layer.axis, -rotStep *
            startStk.direction * rotFactor);
    _facesOrder[2] = {_facesOrder[0].axis, -_facesOrder[0].direction};
    _facesOrder[3] = {_facesOrder[1].axis, -_facesOrder[1].direction};
}

int MatchingPath::getBlockNum(const rcube::Coordinates &blockPos,
    const rcube::Orientation &stk)
{
    if (blockPos == _startBlock && stk == _facesOrder[0]) return 0;

    // count the number of full faces between _startStk and stk
    int counter = 0;
    for (;counter < 4; counter++)
    {
        if (_facesOrder[counter] == stk) break;
    }

    // calculate the position of the sticker within its face
    rcube::Orientation prevFace = _facesOrder[(counter + 3) % 4];
    int pos = blockPos.coords[prevFace.axis];
    int facePos = abs(pos - prevFace.direction);

    return counter * 3 + facePos;
}

bool rcube::Cube::layerMatches(const rcube::Orientation &layer,
    const std::string &expr)
{
    if (expr.size() != 12) return false;

    std::vector<MatchingPath> possiblePaths;
    std::map<char, Color> lmTmp;

    // central and lateral layer are two different cases
    if (layer.direction == 0)
    {
        // first, get a list of all the possible paths
        for (int i = 0; i < 12; ++i)
        {
            if (edges[i].location.coords[layer.axis] != 0) continue;

            for (int k = 0; k < 2; ++k)
            {
                if (stickerMatches(edges[i].stickers[k].color, expr[0], &lmTmp))
                {
                    possiblePaths.push_back(MatchingPath(edges[i].location,
                    edges[i].stickers[k].orientation, layer, lmTmp));
                }
                lmTmp.clear();
            }
        }

        // then, check for each edge and center if it fits in all paths. If not,
        // erase the path from the vector
        for (int i = 0; i < 6; ++i)
        {
            if (centers[i].location.coords[layer.axis] != 0)
                continue;

            for (int x = 0; x < possiblePaths.size(); ++x)
            {
                if (!stickerMatches(centers[i].color, expr[possiblePaths[x]
                    .getBlockNum(centers[i].location, centers[i].orientation)],
                    &possiblePaths[x]._letterMapping))
                {
                    possiblePaths.erase(possiblePaths.begin() + x);
                }
            }
        }
        for (int i = 0; i < 12; ++i)
        {
            if (edges[i].location.coords[layer.axis] != 0) continue;

            for (int k = 0; k < 2; ++k)
            {
                for (int x = 0; x < possiblePaths.size(); ++x)
                {
                    if (!stickerMatches(edges[i].stickers[k].color, expr[
                        possiblePaths[x].getBlockNum(edges[i].location,
                        edges[i].stickers[k].orientation)],
                        &possiblePaths[x]._letterMapping))
                    {
                        possiblePaths.erase(possiblePaths.begin() + x);
                    }
                }
            }
        }
        if (possiblePaths.size() > 0) return true;
        return false;
    }

    // first, get a list of all the possible paths
    for (int i = 0; i < 8; ++i)
    {
        if (corners[i].location.coords[layer.axis] != layer.direction)
            continue;

        for (int k = 0; k < 3; ++k)
        {
            if (corners[i].stickers[k].orientation == layer) continue;

            if (stickerMatches(corners[i].stickers[k].color, expr[0], &lmTmp))
            {
                possiblePaths.push_back(MatchingPath(corners[i].location,
                    corners[i].stickers[k].orientation, layer, lmTmp));
            }
            lmTmp.clear();
        }
    }
    // check for each edge and corner if it fits in all paths. If not,
    // erase the path from the vector
    for (int i = 0; i < 12; ++i)
    {
        if (edges[i].location.coords[layer.axis] == layer.direction)
        {
            for (int k = 0; k < 2; ++k)
            {
                if (edges[i].stickers[k].orientation == layer) continue;

                for (int x = 0; x < possiblePaths.size(); ++x)
                {
                    if (!stickerMatches(edges[i].stickers[k].color, expr[
                        possiblePaths[x].getBlockNum(edges[i].location,
                        edges[i].stickers[k].orientation)],
                        &possiblePaths[x]._letterMapping))
                    {
                        possiblePaths.erase(possiblePaths.begin() + x);
                    }
                }
            }
        }

        if (i >= 8) continue;

        if (corners[i].location.coords[layer.axis] != layer.direction)
            continue;

        for (int k = 0; k < 3; ++k)
        {
            if (corners[i].stickers[k].orientation == layer) continue;

            for (int x = 0; x < possiblePaths.size(); ++x)
            {
                if (!stickerMatches(corners[i].stickers[k].color, expr[
                    possiblePaths[x].getBlockNum(corners[i].location,
                    corners[i].stickers[k].orientation)],
                    &possiblePaths[x]._letterMapping))
                {
                    possiblePaths.erase(possiblePaths.begin() + x);
                }
            }
        }
    }

    if (possiblePaths.size() != 0) return true;
    return false;
}