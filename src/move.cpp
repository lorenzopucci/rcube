/*
 * @file move.cpp
 * @author Lorenzo Pucci
 * @date September 10, 2021
 * @brief File containing the definition of the Move class.
 */

 #include <new.hpp>

rcube::Move::Move(char face, int direction)
{
  this->face = face;
  this->direction = direction;
}

rcube::Move::~Move()
{
  return;
}
