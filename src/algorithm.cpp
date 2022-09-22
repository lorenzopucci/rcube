#include <string>
#include <vector>
#include <sstream>

#include <rcube.hpp>
#include <utility.hpp>

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
            this->algorithm.push_back(Move(fromString[i], CCW));
            ++i;
            break;

          case '2':
            this->algorithm.push_back(Move(fromString[i], DOUBLE));
            ++i;
            break;

          default:
            this->algorithm.push_back(Move(fromString[i], CW));
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

      default:
      {
        throw std::invalid_argument("Invalid algorithm");
        delete this;
      }
     }
   }
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