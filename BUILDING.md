# Building
Using the Makefile is currently the only way to compile the program.

## Make targets
- `lib`: compiles all the files in `/src` without linking them to a main
function. If you want the Lua port to be compiled as well, make sure you have
Lua installed on your machine and compile using `LUA=true`.
- `test`: compiles the library and the test files and links them together.
- `ui`: compiles the library and the frontend and links them together. The
dependencies required here are `glew`, `glfw3` and `glm` (on Ubuntu you would
need the packages `libglew-dev`, `libglfw3-dev` and `libglm-dev`).
- `all`: compiles all of the above.

All the binaries produced will be put in `/bin`. Use `make clean` to empty
that folder.