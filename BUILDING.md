# Building
Using the Makefile is currently the only way to compile the program.

## Make targets
- `lib`: compiles all the files in `/src` without linking them to a main
function. The only dependency required is `lua` (if you do not want the lua
port to be compiled, use `LUA=false`).
- `test`: compiles the library and the test files and links them together.
- `ui`: compiles the library and the frontend and links them together. The
dependencies required here are `glew`, `glfw3` and `glm`.
- `all`: compiles all of the above.

All the binaries produced will be put in `/bin`. Use `make clean` to empty
that folder.