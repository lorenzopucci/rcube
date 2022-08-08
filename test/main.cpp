#include <rcube.hpp>

using namespace rcube;

int main()
{
  Cube cube = Cube();
  cube.display();

  Move move = Move('R', 1);
  cube.performMove(move);
  cube.display();

  //Algorithm algo = Algorithm("RUR'U'");
  //cube.performAlgorithm(algo);
}
