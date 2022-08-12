#include <rcube.hpp>
#include <iostream>

using namespace rcube;

void printBlocks(const rcube::BlockArray& blocks)
{
  for (int i = 0; i < 26; ++i)
  {
    const int* coords = blocks.blocks[i].position.coords;
    std::cout << "Block at (" << coords[0] << ", " << coords[1] << ", " <<
      coords[2] << "):\n";
    
    for (auto it = blocks.blocks[i].stickers.begin();
      it != blocks.blocks[i].stickers.end(); ++it)
    {
      std::cout << "At " << it->first.axis << it->first.direction << ": " <<
        (char)it->second << "\n";
    }
    std::cout << std::endl;
  }
}

int main()
{
  Cube cube = Cube();
  cube.display();

  printBlocks(cube.blockRender());
  

  //Move move = Move('R', 1);
  //cube.performMove(move);
  //cube.display();

  //Algorithm algo = Algorithm("RUR'U'");
  //cube.performAlgorithm(algo);
}
