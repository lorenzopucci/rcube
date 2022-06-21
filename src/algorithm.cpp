/*
 * @file algorithm.cpp
 * @author Lorenzo Pucci
 * @date September 10, 2021
 * @brief File containing the definition of the Algorithm class
 */

 #include <string>
 #include <vector>

 #include <new.hpp>
 #include <definitions.hpp>

rcube::Algorithm::Algorithm(std::string fromString)
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

      case ' ':
      case '(':
      case ')':
      case '[':
      case ']':
      case '\t':
      
        ++i;
        break;
/*
      case '$':

        std::string *algoName = new string("");
        while (fromString[i] =! '$') {
          algoName->push_back(fromString[i]);
          ++i;
        }

        auto *stdAlgo = searchAlgorithm(algoName);
        for (auto move = stdAlgo->begin(); move < stdAlgo->end(); ++move) {
          this->algorithm.push_back(*move);
        }
        ++i;
        break;
      */
      default:

          /*std::cerr << "Error: invalid algorithm: ";
          for (int x = 0; x < fromString.length(); ++x) {
            if (x == i) {
              std::cerr << "\033[31m" << fromString[x] << "\033[0m";
            } else {
              std::cerr << fromString[x];
            } 
          }
          std::cerr << std::endl;*/
          this->algorithm.erase(this->algorithm.begin(), this->algorithm.end());
          break;
     }
   }
 }


std::string rcube::Algorithm::to_string()
{
  std::string stringAlgorithm;

  for (Move move : this->algorithm)
  {
    stringAlgorithm.push_back(move.face);

    switch (move.direction)
    {
      case 1: continue; break;
      case -1: stringAlgorithm.push_back('\''); break;
      case 2: stringAlgorithm.push_back('2'); break;
    }
  }
  return stringAlgorithm;
}


rcube::Algorithm::Algorithm(std::vector<Move> fromVector)
{
  this->algorithm = fromVector;
}


rcube::Algorithm::~Algorithm()
{
  return;
}