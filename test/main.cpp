/*
 * @file main.cpp
 * @author Lorenzo Pucci
 * @date September 10, 2021
 * @brief Main test file
 */

#include <new.hpp>

using namespace rcube;

int main() {
	Cube cube = Cube();
	cube.display();
	cube.performMove(new Move('R', 1));
  cube.display();

/*
  char scramble = 'n';
  std::cout << "Scramble? [Y/n]: ";
  std::cin >> scramble;
  std::cout << std::endl;
  if (scramble == 'Y') {
    cube.scramble();
  }

  while (1) {
    std::string algoString;
    std::cout << "Perform an algorithm ('q' to quit): ";
    std::getline(std::cin, algoString);
    std::cout << std::endl;

    if (algoString == "q") {
      return 0;
    }

    Algorithm *algorithm = new Algorithm(algoString);
    cube.performAlgorithm(algorithm);
    cube.display();
  }*/
}
