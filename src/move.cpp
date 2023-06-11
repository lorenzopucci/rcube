/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <string>
#include <sstream>

#include <rcube.hpp>
#include <utility.hpp>

Axis getAxis(const MoveFace& face)
{
  switch (face)
  {
    case LEFT:
    case RIGHT:
    case ROTATE_X:
    case MIDDLE:
      return Axis::X;

    case UP:
    case DOWN:
    case ROTATE_Y:
    case EQUATOR:
      return Axis::Y;

    case FRONT:
    case BACK:
    case ROTATE_Z:
    case SIDE:
      return Axis::Z;
  }
}

rcube::Move::Move(MoveFace face, MoveDirection direction)
{
  this->face = face;
  this->direction = direction;
  this->axis = getAxis(face);
}

rcube::Move::Move(char face, int direction)
{
  this->face = (MoveFace) face;
  this->direction = (MoveDirection) direction;
  this->axis = getAxis(this->face);
}

rcube::Move::Move(const rcube::Orientation &orient, int direction)
{
  axis = orient.axis;
  this->direction = (MoveDirection) direction;

  switch (orient.axis)
  {
    case 0:
      {
        switch (orient.direction)
        {
          case  1: face = MoveFace::RIGHT; break;
          case -1: face = MoveFace::LEFT; break;
          case  0: face = MoveFace::MIDDLE; break;
        }
        break;
      }
    case 1:
      {
        switch (orient.direction)
        {
          case  1: face = MoveFace::UP; break;
          case -1: face = MoveFace::DOWN; break;
          case  0: face = MoveFace::EQUATOR; break;
        }
        break;
      }
    case 2:
      {
        switch (orient.direction)
        {
          case  1: face = MoveFace::FRONT; break;
          case -1: face = MoveFace::BACK; break;
          case  0: face = MoveFace::SIDE; break;
        }
        break;
      }
  }
}

std::string rcube::Move::to_string() const
{
  std::stringstream ss;
  ss << (char) face;
  switch (direction)
  {
    case CW: break;
    case CCW: ss << '\''; break;
    case DOUBLE: ss << '2'; break;
  }
  return ss.str();
}

rcube::Orientation rcube::Move::getAffectedFace() const
{
  switch (face)
  {
    case UP:
    case RIGHT:
    case FRONT:
      return {axis, 1};

    case DOWN:
    case LEFT:
    case BACK:
      return {axis, -1};
    
    case MIDDLE:
    case SIDE:
    case EQUATOR:
      return {axis, 0};

    default: return {};
  }
}

rcube::Move rcube::Move::getInverted() const
{
  return rcube::Move(face, -direction);
}