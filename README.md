<div align="center">
<img src="doc/icon.png" style="width: 180px">
<h1>Rcube</h1>
</div>
C++ abstraction of a Rubik's cube.

This repository contains:
- A C++ library that includes all the logic to interact with a virtual Rubik's
cube. The headers are in `/include` and the source file are in `/src`.
- An OpenGL frontend to see the cube in 3D and play with it. All the code is in
`/ui`.
- A set of test programs for the library for debugging purposes (in `/test`).

## Using the library
The library's main header file is `/include/rcube.hpp`. It is a long file,
including some internal stuff as well. Nevertheless, everything you should only
care about are three classes:
- `rcube::Cube` to interact with a cube
- `rcube::Move` to apply rotations to the cube
- `rcube::Algorithm` to apply sets of moves to the cube

Use the test scripts as examples.

## Using the frontend
To use the frontend, simply run `make ui`. This will compile the frontend as
well as the backend. Make sure you have all the necessary dependencies: `glew`,
`glfw3` and `glm`.

## Make targets
- `lib`: compiles all the files in `/src` without linking them to a main
function.
- `test`: compiles the library and the test files and links them together.
- `ui`: compiles the library and the frontend and links the together.

All the binary files produced will be put in `/bin`. Use `make clean` to empty
that folder.

*A work in progress by Lorenzo Pucci*