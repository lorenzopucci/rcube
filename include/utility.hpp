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
