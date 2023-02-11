/*
* Copyright (c) 2023 Lorenzo Pucci
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
#include "./solving/algoList.hpp"

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
        }

        i += 2;
        std::stringstream ss;

        while (i < fromString.length() && fromString[i] != ')')
        {
          ss << fromString[i];
          i++;
        }

        bool found = false;
        for (AlgoDBItem item : algoDb)
        {
          if (item.name != ss.str()) continue;

          found = true;
          std::vector<rcube::Move> algo = rcube::Algorithm(item.algo).algorithm;
          algorithm.insert(algorithm.end(), algo.begin(), algo.end());
          break;
        }
        if (!found)
        {
          throw std::invalid_argument("Cannot find algorithm: " + ss.str());
        }

        i++;
        break;
      }

      default:
      {
        throw std::invalid_argument("Invalid algorithm");
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
  if (algorithm.size() == 0) return;
  
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

void editFaceMap(std::map<rcube::Orientation, rcube::Orientation> *faceMap,
  Axis axis, int direction)
{
  for (auto it = faceMap->begin(); it != faceMap->end(); ++it)
  {
    it->second.rotate(axis, direction);
  }
}

void removeRotationsUnit(const rcube::Algorithm &algo, rcube::Algorithm *newAlgo,
  std::map<rcube::Orientation, rcube::Orientation> *faceMap)
{
  for (rcube::Move mv : algo.algorithm)
  {
    switch (mv.face)
    {
      case ROTATE_X:
        editFaceMap(faceMap, Axis::X, mv.direction); break;
      case ROTATE_Y:
        editFaceMap(faceMap, Axis::Y, mv.direction); break;
      case ROTATE_Z:
        editFaceMap(faceMap, Axis::Z, mv.direction); break;

      case MIDDLE:
      case SIDE:
      case EQUATOR:
      {
        std::map<MoveFace, std::string> algos = {
          {MIDDLE, "RL'x'"},
          {EQUATOR, "UD'y'"},
          {SIDE, "F'Bz"}
        };

        rcube::Algorithm expansion(algos[mv.face]);
        removeRotationsUnit(expansion * mv.direction, newAlgo, faceMap);
      }

      case UP_W:
      case FRONT_W:
      case RIGHT_W:
      case LEFT_W:
      case DOWN_W:
      case BACK_W:
      {
        std::map<MoveFace, std::string> algos = {
          {UP_W, "Dy"},
          {FRONT_W, "Bz"},
          {RIGHT_W, "Lx"},
          {LEFT_W, "Rx'"},
          {DOWN_W, "Uy'"},
          {BACK_W, "Fz'"}
        };

        rcube::Algorithm expansion(algos[mv.face]);
        removeRotationsUnit(expansion * mv.direction, newAlgo, faceMap);
      }

      default:
        rcube::Orientation affFace = mv.getAffectedFace();
        for (auto it = faceMap->begin(); it != faceMap->end(); ++it)
        {
          if (it->second == affFace)
            newAlgo->push(rcube::Move(it->first, mv.direction));
        }
        break;
    }
  }
}

rcube::Algorithm rcube::Algorithm::removeRotations() const
{
  rcube::Algorithm newAlgo;
  std::map<rcube::Orientation, rcube::Orientation> faceMap;

  for (rcube::Orientation ort : rcube::Orientation::iterate())
    faceMap.insert(std::pair<rcube::Orientation, rcube::Orientation>(ort, ort));

  removeRotationsUnit(algorithm, &newAlgo, &faceMap);

  newAlgo.normalize();
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