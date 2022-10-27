# Lua integration

Most of the C++ API is also available for Lua. To run some Lua code, use the
following functions:
- [`int rcube::Cube::runScript(const std::string &path)`](cube.md)
- [`int rcube::Cube::runCommand(const std::string &cmd)`](cube.md)

This file contains a list of all the Lua functions available.


### `<string> scramble(<number> length)`

- `length`: the random algorithm's length;

Equivalent to `rcube::Cube::scramble`. The generated algorithm is returned as
a string.


### `performMove(<string> move, <number> direction)`

- `move`: the move to perform (`U`, `R`, `M`, `x`...);
- `direction`: `+1` for clockwise, `-1` for counterclockwise, `2` for double;

Equivalent to `rcube::Cube::performMove`. Does not return anything.


### `performAlgorithm(<string> algorithm)`

- `algorithm`: the algorithm to perform;

Equivalent to `rcube::Cube::performAlgorithm`. Does not return anything.


### `<boolean> isSolved()`

Equivalent to `rcube::Cube::isSolved`. Returns `true` when the cube is solved.


### `<boolean> faceMatches(<number> axis, <number> direction, <string> expr)`

- `axis`: along with `direction` determines the face to check. Use `0` for `X`,
 `1` for `Y` and `2` for `Z`;
- `direction` along with `axis` determines the face to check;
- `expr`: the regular expression to compare with the face. For its syntax
[read here](cube.md)

Equivalent to `rcube::Cube::faceMatches`.


### `<boolean> layerMatches(<number> axis, <number> direction, <string> expr)`

- `axis`: along with `direction` determines the layer to check. Use `0` for `X`,
 `1` for `Y` and `2` for `Z`;
- `direction` along with `axis` determines the layer to check;
- `expr`: the regular expression to compare with the face. For its syntax
[read here](cube.md)

Equivalent to `rcube::Cube::layerMatches`