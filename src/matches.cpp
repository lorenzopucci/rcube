/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <rcube.hpp>

#include "cfop/util.hpp"

bool handleLetters(Color color, char given, char otherCase,
    std::map<char, Color> *letterMapping)
{
    // This is the part of stickerMatches which handles letters that do not
    // indicate a color directly (i.e. variables)

    // Letters of type A = {A,...,L}
    // Letters of type B = {M,...,Z} (see documentation)

    bool isTypeB = (int)given >= 77 && (int)otherCase >= 77;

    if (letterMapping->find(given) != letterMapping->end())
    {
        return color == letterMapping->at(given);
    }
    else if (letterMapping->find(otherCase) != letterMapping->end())
    {
        if (isTypeB)
        {
            Color opposite = getOppositeColor(letterMapping->at(otherCase));
            letterMapping->insert(std::pair<char, Color>(given, opposite));
            return color == opposite;
        }

        return color != letterMapping->at(otherCase);
    }

    // Neither given, nor otherCase has been found: needs to be added

    if (isTypeB)
    {
        // make sure no other letter is pointing to that color
        for (auto it : *letterMapping)
        {
            if (it.second == color && tolower(it.first) >= 109) return false;
        }
    }

    letterMapping->insert(std::pair<char, Color>(given, color));
    return true;
}

bool stickerMatches(const Color &color, const char &match,
    std::map<char, Color> *letterMapping)
{
    // Returns true if <match> (a character from the regex) matches color <color>
    // according to <letterMapping>. If <letterMapping> does not include that
    // character, it will be added. False is returned only when the pair
    // (<match>, <color>) conflicts with the data in <letterMapping>.

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

int getCharPos(const rcube::Coordinates2D &coords)
{
    // Maps 2D coordinates of a face to indices in a faceMatches regex

    return ((abs(coords.coords[Axis::Y] - 1)) * 3) + coords.coords[Axis::X] + 1;
}

bool rcube::Cube::faceMatches (
    const rcube::Orientation &face,
    const std::string &expr,
    const rcube::Coordinates &dest,
    rcube::Algorithm *algo
)
{
    // check the syntax
    if (expr.size() != 9) return false;

    // stores which color each letter represents
    std::map<char, Color> letterMapping;

    // check the center
    if (!stickerMatches(getCenterFrom(face)->color, expr[4], &letterMapping))
        return false;


    for (int rot = 0; rot < 4; ++rot) // try each rotation until a match is found
    {
        letterMapping.clear();

        // if the center is a letter, it must be reincluded in the map
        stickerMatches(getCenterFrom(face)->color, expr[4], &letterMapping);

        rcube::Corner *first; // Pointer to the corner corresponding to expr[0]

        // Iterate through corners and edges to check if all of them match
        for (int i = 0; i < 12; ++i)
        {
            for (int k = 0; k < 2; ++k)
            {
                if (edges[i].stickers[k].orientation != face) continue;

                rcube::Coordinates2D patternCoords(edges[i].location, face);
                patternCoords.rotate(rot);

                if (!stickerMatches(edges[i].stickers[k].color,
                    expr[getCharPos(patternCoords)], &letterMapping))
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
                int pos = getCharPos(patternCoords);

                if (pos == 0) first = corners + i;

                if (!stickerMatches(corners[i].stickers[k].color,
                    expr[pos], &letterMapping))
                        goto nextIt;
            }
        }

        // Move <first> to desired position (if given)
        if (dest != rcube::Coordinates(0, 0, 0))
        {
            if (dest.coords[face.axis] != face.direction) return true;

            rcube::Move mv(face, 1);

            int i = 0;
            for (; i < 4; ++i) // Apply move until desired position is reached
            {
                if (first->location == dest) break;
                performMove(mv);
            }
            if (algo != nullptr)
            {
                if (i == 3) i = -1;
                algo->push(rcube::Move(face, i));
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
    // A MatchingPath is one of the ways in which a regex can match a layer.
    // It is determined by a starting sticker (coordinates + orientation) and
    // by the face around which the matching algorithm is performed (layer)

    rcube::Coordinates _startBlock = rcube::Coordinates(0,0,0);
    rcube::Orientation _layer;
    rcube::Orientation _facesOrder[4];

    std::map<char, Color> _letterMapping; // gets fed to stickerMatches()

    MatchingPath(const rcube::Coordinates &startBlock, const rcube::Orientation
        &startStk, const rcube::Orientation &layer,
        const std::map<char, Color> &lm);

    // get the distance of a block from the starting point
    int getBlockNum(const rcube::Coordinates &blockPos,
        const rcube::Orientation &stk);

    int getFaceBlockNum(const rcube::Coordinates &pos);
};

MatchingPath::MatchingPath(const rcube::Coordinates &startBlock, const
    rcube::Orientation &startStk, const rcube::Orientation &layer,
        const std::map<char, Color> &lm)
{
    _startBlock = startBlock;
    _layer = layer;
    _letterMapping = lm;

    // rotStep and rotFactors are defined so that (-rotStep * rotFactor *
    // startSt.direction) is the number of 90° twists to turn the face of
    // expr[0,1,2] to that of expr[3,4,5]

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

int MatchingPath::getFaceBlockNum(const rcube::Coordinates &pos)
{
    int row = abs(pos.coords[_facesOrder[0].axis] -
        _startBlock.coords[_facesOrder[0].axis]);
    int column = abs(pos.coords[_facesOrder[3].axis] -
        _startBlock.coords[_facesOrder[3].axis]);
    
    return row * 3 + column;
}

bool rcube::Cube::layerMatches(
    const rcube::Orientation &layer,
    const std::string &expr,
    const rcube::Coordinates &dest,
    const rcube::Orientation &orient,
    rcube::Algorithm *algo
)
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
        for (int i = 0; i < 6; ++i) // centers
        {
            if (centers[i].location.coords[layer.axis] != 0)
                continue;

            for (int x = 0; x < possiblePaths.size();)
            {
                if (!stickerMatches(centers[i].color, expr[possiblePaths[x]
                    .getBlockNum(centers[i].location, centers[i].orientation)],
                    &possiblePaths[x]._letterMapping))
                {
                    possiblePaths.erase(possiblePaths.begin() + x);
                    continue;
                }
                ++x;
            }
        }
        for (int i = 0; i < 12; ++i) // edges
        {
            if (edges[i].location.coords[layer.axis] != 0) continue;

            for (int k = 0; k < 2; ++k)
            {
                for (int x = 0; x < possiblePaths.size();)
                {
                    if (!stickerMatches(edges[i].stickers[k].color, expr[
                        possiblePaths[x].getBlockNum(edges[i].location,
                        edges[i].stickers[k].orientation)],
                        &possiblePaths[x]._letterMapping))
                    {
                        possiblePaths.erase(possiblePaths.begin() + x);
                        continue;
                    }
                    ++x;
                }
            }
        }

        if (possiblePaths.size() == 0) return false;

        if (dest == rcube::Coordinates(0, 0, 0)) return true;
        if (dest.coords[layer.axis] != layer.direction) return true;

        // move the first edge to the desired position (if given)

        rcube::Move mv(layer, 1);
        int moveCount = 0;

        for (int i = 0; i < 12; ++i)
        {
            if (edges[i].location.coords[layer.axis] != layer.direction)
                continue;

            // there might be multiple paths, however some may result in the
            // first edge not aligning correctly with with <orient>. If one
            // satisfies this condition, it will be chosen

            for (auto path : possiblePaths)
            {
                if (edges[i].location != path._startBlock) continue;

                rcube::Sticker *startStk; // sticker corresponding to expr[1]
                for (int x = 0; x < 2; ++x)
                {
                    if (edges[i].stickers[x].orientation == path._facesOrder[0])
                        startStk = edges[i].stickers + x;
                }

                for (int x = 0; x < 4; ++x)
                {
                    performMove(mv);
                    moveCount++;

                    if (edges[i].location != dest) continue;

                    if (orient == (rcube::Orientation){Axis::X, 0})
                        return true;
                    
                    if (startStk->orientation == orient)
                    {
                        if (algo != nullptr)
                        {
                            moveCount = moveCount % 4;
                            if (moveCount == 3) moveCount = -1;
                            algo->push(rcube::Move(layer, moveCount));
                        }
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // Case in which the layer is next to a face (i.e. not central):

    // first, get a list of all possible paths
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

                for (int x = 0; x < possiblePaths.size();)
                {
                    if (!stickerMatches(edges[i].stickers[k].color, expr[
                        possiblePaths[x].getBlockNum(edges[i].location,
                        edges[i].stickers[k].orientation)],
                        &possiblePaths[x]._letterMapping))
                    {
                        possiblePaths.erase(possiblePaths.begin() + x);
                        continue;
                    }
                    ++x;
                }
            }
        }

        if (i >= 8) continue;

        if (corners[i].location.coords[layer.axis] != layer.direction)
            continue;

        for (int k = 0; k < 3; ++k)
        {
            if (corners[i].stickers[k].orientation == layer) continue;

            for (int x = 0; x < possiblePaths.size();)
            {
                if (!stickerMatches(corners[i].stickers[k].color, expr[
                    possiblePaths[x].getBlockNum(corners[i].location,
                    corners[i].stickers[k].orientation)],
                    &possiblePaths[x]._letterMapping))
                {
                    possiblePaths.erase(possiblePaths.begin() + x);
                    continue;
                }
                ++x;
            }
        }
    }

    if (possiblePaths.size() == 0) return false;

    if (dest == rcube::Coordinates(0, 0, 0)) return true;
    if (dest.coords[layer.axis] != layer.direction) return true;

    // move the first block to the desired position

    rcube::Move mv(layer, 1);
    int moveCount = 0;

    for (int i = 0; i < 8; ++i)
    {
        if (corners[i].location.coords[layer.axis] != layer.direction) continue;

        for (auto path : possiblePaths)
        {
            if (corners[i].location != path._startBlock) continue;

            rcube::Sticker *startStk; // sticker corresponding to expr[0]
            for (int x = 0; x < 3; ++x)
            {
                if (corners[i].stickers[x].orientation == path._facesOrder[0])
                {
                    startStk = corners[i].stickers + x;
                }
            }

            for (int x = 0; x < 4; ++x)
            {
                performMove(mv);
                moveCount++;
                if (corners[i].location != dest) continue;

                if (startStk->orientation == orient)
                {
                    if (algo != nullptr)
                    {
                        moveCount = moveCount % 4;
                        if (moveCount == 3) moveCount = -1;
                        algo->push(rcube::Move(layer, moveCount));
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool rcube::Cube::layerAndFaceMatch(
    const rcube::Orientation &layer,
    const std::string &expr,
    const rcube::Coordinates &dest,
    const rcube::Orientation &orient,
    rcube::Algorithm *algo
)
{
    if (expr.size() != 22 || layer.direction == 0) return false;

    std::string faceExpr = expr.substr(0, 9);
    std::string layerExpr = expr.substr(10, 12);

    // The code is very similar to layerMatches
    std::vector<MatchingPath> possiblePaths;
    std::map<char, Color> lmTmp;

    // check the center
    if (!stickerMatches(getCenterFrom(layer)->color, faceExpr[4], &lmTmp))
        return false;
    lmTmp.clear();

    // first, get a list of all the possible paths
    for (int i = 0; i < 8; ++i)
    {
        if (corners[i].location.coords[layer.axis] != layer.direction)
            continue;
        
        for (int k = 0; k < 3; ++k) // check the sticker belonging to the face
        {
            if (corners[i].stickers[k].orientation != layer) continue;

            lmTmp.clear();
            stickerMatches(getCenterFrom(layer)->color, faceExpr[4], &lmTmp);

            if (!stickerMatches(corners[i].stickers[k].color, faceExpr[0], &lmTmp))
            {
                goto nextBlock;
            }
            break;
        }

        lmTmp.clear();
        stickerMatches(getCenterFrom(layer)->color, faceExpr[4], &lmTmp);

        for (int k = 0; k < 3; ++k)
        {
            if (corners[i].stickers[k].orientation == layer) continue;

            if (stickerMatches(corners[i].stickers[k].color, layerExpr[0], &lmTmp))
            {
                possiblePaths.push_back(MatchingPath(corners[i].location,
                    corners[i].stickers[k].orientation, layer, lmTmp));
            }

            lmTmp.clear();
            stickerMatches(getCenterFrom(layer)->color, faceExpr[4], &lmTmp);
        }
    nextBlock:;
    }

    // check for each edge and corner if it fits in all paths. If not,
    // erase the path from the vector
    for (int i = 0; i < 12; ++i)
    {
        if (edges[i].location.coords[layer.axis] == layer.direction)
        {
        for (int k = 0; k < 2; ++k) // edges
        {
            rcube::Sticker *stk = edges[i].stickers + k;

            for (int x = 0; x < possiblePaths.size();)
            {
                // the sticker belongs to the face
                if (stk->orientation == layer && stickerMatches(stk->color,
                    faceExpr[possiblePaths[x].getFaceBlockNum(edges[i].location)],
                    &possiblePaths[x]._letterMapping))
                {
                    ++x;
                    continue;
                }

                // the sticker belongs to the layer
                if (stk->orientation != layer && stickerMatches(stk->color,
                    layerExpr[possiblePaths[x].getBlockNum(edges[i].location,
                    stk->orientation)], &possiblePaths[x]._letterMapping)
                )
                {
                    ++x;
                    continue;
                }
                possiblePaths.erase(possiblePaths.begin() + x);
            }
        }
        }

        if (i >= 8) continue;

        if (corners[i].location.coords[layer.axis] != layer.direction)
            continue;

        for (int k = 0; k < 3; ++k) // corners
        {
            rcube::Sticker *stk = corners[i].stickers + k;

            for (int x = 0; x < possiblePaths.size();)
            {
                // the sticker belongs to the face
                if (stk->orientation == layer && stickerMatches(stk->color,
                    faceExpr[possiblePaths[x].getFaceBlockNum(corners[i].location)],
                    &possiblePaths[x]._letterMapping))
                {
                    ++x;
                    continue;
                }

                // the sticker belongs to the layer
                if (stk->orientation != layer && stickerMatches(stk->color,
                    layerExpr[possiblePaths[x].getBlockNum(corners[i].location,
                    stk->orientation)], &possiblePaths[x]._letterMapping)
                )
                {
                    ++x;
                    continue;
                }
                possiblePaths.erase(possiblePaths.begin() + x);
            }
        }
    }

    if (possiblePaths.size() == 0) return false;

    if (dest == rcube::Coordinates(0, 0, 0)) return true;
    if (dest.coords[layer.axis] != layer.direction) return true;

    // move the first block to the desired position

    rcube::Move mv(layer, 1);
    int moveCount = 0;

    for (int i = 0; i < 8; ++i)
    {
        if (corners[i].location.coords[layer.axis] != layer.direction) continue;

        for (auto path : possiblePaths)
        {
            if (corners[i].location != path._startBlock) continue;

            rcube::Sticker *startStk; // sticker corresponding to expr[1]
            for (int x = 0; x < 3; ++x)
            {
                if (corners[i].stickers[x].orientation == path._facesOrder[0])
                {
                    startStk = corners[i].stickers + x;
                }
            }

            for (int x = 0; x < 4; ++x)
            {
                performMove(mv);
                moveCount++;
                if (corners[i].location != dest) continue;

                if (startStk->orientation == orient)
                {
                    if (algo != nullptr)
                    {
                        moveCount = moveCount % 4;
                        if (moveCount == 3) moveCount = -1;
                        algo->push(rcube::Move(layer, moveCount));
                    }
                    return true;
                }
            }
        }
    }
    return false;
}