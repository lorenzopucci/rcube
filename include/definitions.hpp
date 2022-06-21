/*
 * @file definitions.hpp
 * @author Lorenzo Pucci
 * @date September 10, 2021
 * @brief File containing the definitions of the following entities:
 *  Colours: the 6 colours of the stickers
 *  Faces: the 6 faces of the cube (referenced by location)
 *  Moves: the 6 faces that can be rotated
 *  Move directions: the directions of the faces' rotations
 *  Stickers: the 8 stickers (not including the center) of each face (referenced by location)
 */

#ifndef COLOURS
#define COLOURS

#define WHITE 'w'
#define YELLOW 'y'
#define GREEN 'g'
#define BLUE 'b'
#define RED 'r'
#define ORANGE 'o'


// order in rcube::Cube::centers
#define WHITE_CENTER 0
#define YELLOW_CENTER 1
#define GREEN_CENTER 2
#define BLUE_CENTER 3
#define RED_CENTER 4
#define ORANGE_CENTER 5

#endif

#ifndef FACES
#define FACES
/*
#define YELLOW_FACE 0
#define GREEN_FACE 1
#define ORANGE_FACE 2
#define RED_FACE 3
#define WHITE_FACE 4
#define BLUE_FACE 5*/

#define TOP_FACE 0
#define LEFT_FACE 1
#define FRONT_FACE 2
#define RIGHT_FACE 3
#define BACK_FACE 4
#define BOTTOM_FACE 5

#endif

#ifndef  MOVES
#define MOVES

#define RIGHT 'R'
#define LEFT 'L'
#define UP 'U'
#define DOWN 'D'
#define FRONT 'F'
#define BACK 'B'
#define MIDDLE 'M'
#define EQUATOR 'E'
#define SIDE 'S'
#define ROTATE_X 'x'
#define ROTATE_Y 'y'
#define ROTATE_Z 'z'

#define CW 1
#define CCW -1
#define DOUBLE 2

#endif
/*
#ifndef STICKERS
#define STICKERS

#define TOP_LEFT 0
#define TOP_CENTER 1
#define TOP_RIGHT 2
#define CENTER_RIGHT 3
#define BOTTOM_RIGHT 4
#define BOTTOM_CENTER 5
#define BOTTOM_LEFT 6
#define CENTER_LEFT 7

#endif
*/