/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <string>
#include <vector>
#include <sstream>

#include <rcube.hpp>
#include <utility.hpp>
#include "algoList.hpp"

rcube::Algorithm::Algorithm(const std::string& fromString)
{
  for (int i = 0; i < fromString.length(); ++i) {

    switch (fromString[i]) {
      case UP:
      case FRONT:
      case RIGHT:
      case LEFT:
      case DOWN:
      case BACK:

      case UP_W:
      case FRONT_W:
      case RIGHT_W:
      case LEFT_W:
      case DOWN_W:
      case BACK_W:

      case MIDDLE:
      case EQUATOR:
      case SIDE:

      case ROTATE_X:
      case ROTATE_Y:
      case ROTATE_Z:
      {
        switch (fromString[i+1])
        {
          case '\'':
            push(Move(fromString[i], CCW));
            ++i;
            break;

          case '2':
            push(Move(fromString[i], DOUBLE));
            ++i;
            break;

          default:
            push(Move(fromString[i], CW));
            break;
        }
        break;
      }

      case ' ':
      case '(':
      case ')':
      case '[':
      case ']':
      case '\t':
      {
        break;
      }

      case '$':
      {
        if (fromString[i+1] != '(')
        {
          throw std::invalid_argument("Invalid algorithm");
          delete this;
        }

        i += 2;
        std::stringstream ss;

        while (fromString[i] != ')')
        {
          ss << fromString[i];
          i++;
        }
        if (algoDb.find(ss.str()) == algoDb.end())
        {
          throw std::invalid_argument("Cannot find algorithm: " + ss.str());
          delete this;
        }

        for (rcube::Move move : rcube::Algorithm(algoDb[ss.str()]).algorithm)
        {
          push(move);
        }

        i++;
        break;
      }

      default:
      {
        throw std::invalid_argument("Invalid algorithm");
        delete this;
      }
     }
   }
 }

rcube::Algorithm::Algorithm(const std::vector<rcube::Move>& fromVector)
{
  this->algorithm = fromVector;
}

rcube::Algorithm rcube::Algorithm::operator*(const int& factor)
{
  rcube::Algorithm result = rcube::Algorithm(algorithm);

  for (auto it = result.algorithm.begin(); it < result.algorithm.end(); ++it)
  {
    int newDirection = (factor * it->direction) % 4;
    if (newDirection < -1) newDirection += 4;
    if (newDirection == 0) result.algorithm.erase(it);

    it->direction = static_cast<MoveDirection>(newDirection);
  }
  return result;
}

rcube::Algorithm rcube::Algorithm::operator+(const rcube::Algorithm a)
{
  std::vector<rcube::Move> dest = algorithm;
  dest.insert(dest.end(), a.algorithm.begin(), a.algorithm.end());
  return rcube::Algorithm(dest);
}

inline void rcube::Algorithm::push(const rcube::Move &m)
{
  algorithm.push_back(m);
}

rcube::Algorithm rcube::Algorithm::generateScramble(const int &length)
{
  rcube::Algorithm scramble;
  srand(time(0));

  MoveFace moves[6] = {RIGHT, LEFT, UP, DOWN, FRONT, BACK};
  MoveFace prev;

  while (scramble.length() <= length)
  {
    MoveFace move = moves[rand() % 6];
    while (move == prev)
    {
      move = moves[rand() % 6];
    }

    int dir = rand() % 3; // {0, 1, 2}
    if (dir == 0) dir = -1; // {-1, 1, 2}

    scramble.push(rcube::Move(move, static_cast<MoveDirection>(dir)));
    prev = move;
  }
  return scramble;
}

void rcube::Algorithm::normalize()
{
  for (auto it = algorithm.begin(); it < algorithm.end() - 1;)
  {
    if (it->face != (it + 1)->face)
    {
      ++it;
      continue;
    }

    int newDir = (int)it->direction + (int)(it + 1)->direction;
    newDir = (newDir + 4) % 4;
    if (newDir == 3) newDir = -1;

    it->direction = static_cast<MoveDirection>(newDir);
    
    algorithm.erase(it + 1);

    if (newDir == 0)
    {
      algorithm.erase(it);
    }
  }
}

rcube::Algorithm rcube::Algorithm::reverse() const
{
  rcube::Algorithm newAlgo;

  for (auto it = algorithm.end() - 1; it >= algorithm.begin(); --it)
  {
    int newDir = (int)it->direction;
    if (abs(newDir) == 1) newDir *= -1;

    newAlgo.push(rcube::Move(it->face, newDir));
  }

  return newAlgo;
}

std::string rcube::Algorithm::to_string() const
{
  std::stringstream ss;

  for (rcube::Move move : algorithm)
  {
    ss << (char) move.face;

    switch (move.direction)
    {
      case 1: break;
      case -1: ss << '\''; break;
      case 2: ss << '2'; break;
    }
  }
  return ss.str();
}

inline int rcube::Algorithm::length() const
{
  return algorithm.size();
}