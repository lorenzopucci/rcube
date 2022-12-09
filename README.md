<div align="center">
<img src="icon.png" style="width: 180px">
<h1>Rcube</h1>
</div>
C++ abstraction of a Rubik's cube.

This repository contains:
- A C++ library that includes all the logic to interact with a virtual Rubik's
cube. The headers are in `/include` and the source files are in `/src`.
- An OpenGL frontend to see the cube in 3D and play with it. All the code is in
`/ui`.
- A set of test programs for the library for debugging purposes (in `/test`).

## Features
- [x] Basic interaction with a `3*3*3` cube;
- [x] Rendering in multiple formats;
- [x] Expandibility with Lua scripts (most of the C++ API is also available
in Lua);
- [x] Cube status retrieval in multiple and advanced ways (such as regual
expressions);
- [ ] Solving algorithms;
- [ ] Interaction with cubes of different sizes (`2*2*2`, `4*4*4`...);

## Using the library
The library's main header file is `/include/rcube.hpp`. It is a long file,
including some internal stuff as well. Nevertheless, everything you should
care about are only three classes:
- `rcube::Cube` to interact with a cube
- `rcube::Move` to apply rotations to the cube
- `rcube::Algorithm` to apply sets of moves to the cube

[Continue reading](doc/README.md)

## Make targets
- `lib`: compiles all the files in `/src` without linking them to a main
function. The only dependency required is `lua` (if you do not want the lua
port to be compiled, use `LUA=false`).
- `test`: compiles the library and the test files and links them together.
- `ui`: compiles the library and the frontend and links them together. The
dependencies required here are `glew`, `glfw3` and `glm`.

All the binaries produced will be put in `/bin`. Use `make clean` to empty
that folder.

<br>

*A work in progress by Lorenzo Pucci*