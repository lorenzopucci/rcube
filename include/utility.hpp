/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

enum class Color : char
{
    White = 'w',
    Yellow = 'y',
    Green = 'g',
    Blue = 'b',
    Red = 'r',
    Orange = 'o'
};

enum MoveFace
{
    RIGHT = 'R',
    LEFT = 'L',
    UP = 'U',
    DOWN = 'D',
    FRONT = 'F',
    BACK = 'B',

    RIGHT_W = 'r',
    LEFT_W = 'l',
    UP_W = 'u',
    DOWN_W = 'd',
    FRONT_W = 'f',
    BACK_W = 'b',

    MIDDLE = 'M',
    EQUATOR = 'E',
    SIDE = 'S',
    
    ROTATE_X = 'x',
    ROTATE_Y = 'y',
    ROTATE_Z = 'z'
};

enum MoveDirection
{
    CW = 1,
    CCW = -1,
    DOUBLE = 2
};

enum Axis
{
    X = 0, Y = 1, Z = 2
};
