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

enum class CenterId : int // indices within rcube::Cube::centers
{
    White = 0,
    Yellow = 1,
    Green = 2,
    Blue = 3,
    Red = 4,
    Orange = 5
};
/*
enum ViewpointIndices // within rcube::Cube::viewpoint
{
    TOP_FACE = 0,
    LEFT_FACE = 1,
    FRONT_FACE = 2,
    RIGHT_FACE = 3,
    BACK_FACE = 4,
    BOTTOM_FACE = 5
};*/

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
