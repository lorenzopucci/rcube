<div align="center">
<img src="icon.png" style="width: 180px">
<h1>Rcube</h1>
</div>

This repository contains:
- A C++ library that includes all the logic to interact with a virtual Rubik's
cube, including some solving algorithms. The headers are in `/include` and the
source files are in `/src`.
- An OpenGL frontend to see the cube in 3D and play with it. All the code is in
`/ui`.
- A sketchy implementation of the library for a robot solver (in `/robot`).

## References
- [Library's documentation](doc/README.md)
- [Building instructions](BUILDING.md)
- [GUI details](ui/README.md)
- [Robot details](robot/README.md)

The following content of this file is only about the library.

## Features
- [x] Basic interaction with a `3*3*3` cube;
- [x] Rendering in multiple formats;
- [x] Expandibility with Lua scripts (most of the C++ API is also available
in Lua);
- [x] Cube status retrieval in multiple and advanced ways (such as regual
expressions);
- [x] Algorithm database and manipulation;
- [x] Human-like solver;
- [x] Optimal solver (currently Kociemba algorithm);
- [ ] Support of cubes of different sizes (`2*2*2`, `4*4*4`...);

## Example
Here is a code snippet that shows a few of the library's capabilities:

```cpp
#include <rcube.hpp>

int main()
{
    rcube::Cube cube();
    rcube::Algorithm algo("F$(sexy-move)F'");

    cube.performAlgorithm(algo);
    cube.performAlgorithm(algo.reverse());
    assert(cube.isSolved());

    cube.scarmble(18);
    rcube::Algorithm solution = cube.solveKociemba();

    cube.runScript("test/main.lua");
    cube.display();
}
```