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
    std::map<char, Color> *letterMapping = {})
{
    if (match == '*') return true;
    
    switch (match)
    {
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

bool rcube::Cube::matches(const rcube::Orientation &face,
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