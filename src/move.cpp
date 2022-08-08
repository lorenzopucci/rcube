#include <string>

#include <rcube.hpp>
#include <utility.hpp>

Axis getAxis(const MoveFace& face)
{
  switch (face)
  {
    case LEFT:
    case RIGHT:
    case ROTATE_X:
      return Axis::X; break;
    case UP:
    case DOWN:
    case ROTATE_Y:
      return Axis::Y; break;
    case FRONT:
    case BACK:
    case ROTATE_Z:
      return Axis::Z; break;
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

std::string rcube::Move::to_string() const
{
  std::string result;
  result.push_back((char) face);
  switch (direction)
  {
    case CW: break;
    case CCW: result.push_back('\''); break;
    case DOUBLE: result.push_back('2'); break;
  }
  return result;
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

    default: return {};
  }
}