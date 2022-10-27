# `rcube::Cube`
This class represents a $3\times3\times3$ Rubik's cube.

### `rcube::Cube::Cube(const Color& topColor = Color::White, const Color& frontColor = Color::Green)`

- `topColor`: the color of the top face (default: `Color::White`)
- `frontColor`: the color of the front face (default: `Color::Green`)

This is the default and only constructor. It initializes a solved cube with the
orientation specified by `topColor` and `frontColor`.

NOTE: this constructor may result in a program crash (for assertion failure) if
`topColor` and `fronColor` are not two adjacent colors (for example when one is
white and the other is yellow).


### `void rcube::Cube::performMove(const rcube::Move &move)`

- `move`: the move to perform (see [`rcube::Move`](move.md));

Applies a move to the cube. A move can be the rotation of a layer or the
rotation of the cube itself (change of viewpoint).


### `void rcube::Cube::performAlgorithm (const rcube::Algorithm& algorithm)`

- `algorithm`: the algorithm to perform (see [`rcube::Algorithm`](algorithm.md));

Applies an algorithm (set of moves) to the cube.


### `bool rcube::Cube::isSolved()`
Returns `true` when the cube is solved (all the blocks are in the right place).


### `void rcube::Cube::scramble(const int &length = 12, rcube::Algorithm* dest = nullptr)`

- `length`: the scramble's length (number of random moves, default: `12`);
- `dest`: a pointer to an algorithm where the generated algorithm will be
stored (default: `nullptr`);

Applies a randomly generated algorithm of the given size. The algorithm can be
retrieved by specifying a valid pointer.


### `rcube::Cube::faceMatches(const rcube::Orientation &face, const std::string &expr)`

- `face`: the face to check (expressed with its orientation);
- `expr`: the regualr expression;

Returns `true` when the stickers of the given face match the given pattern.
The pattern is a `std::string` with the following syntax:

- Every star (`*`) indicates a sticker that can be of any color;
- The uppercase letters `W`, `Y`, `G`, `B`, `R`, `O` indicate a sticker of the
corresponding color (`W` for white, `Y` for yellow and so on);
- The lowercase letters `w`, `y`, `g`, `b`, `r`, `o` indicate a sticker that
is NOT of the corresponding color (`w` is every color but white);
- All the other letters indicate a sticker of an unspecified color, but with
the following rules:
    - Different letters correspond to different colors;
    - A pair of uppercase and lowercase letters correspond to opposite colors
    (if `A` is blue, `a` is green);

The face is read left-to-right, top-to-bottom:
```
             123
123456789 -> 456
             789
```
For example, a cross is `*A*AAA*A*`

Note: the string MUST be 9 characters long, otherwise it will not be accepted.


### `rcube::Cube::layerMatches(const rcube::Orientation &layer, const std::string &expr)`

- `layer`: the layer to check (expressed with the orientation of its adjacent
face. For middle layer, use `layer.direction=0` even though
`rcube::Orientation` is not designed for that value);
- `expr`: the regular expression;

Returns `true` when the stickers of the given layer match the given pattern.
The term layer means the set of stickers adjacent to the same face or one of
the middle layers (the ones to which the moves `M`, `S` and `E` are applied).

The regualr expession's syntax is the same as in `rcube::cube::faceMatches`
but instead of 9, it must be 12 characters long.


### `rcube::Net rcube::Cube::netRender()`

Returns a 2D representation of the cube (basically a
`rcube::Orientation → rcube::Face` map, where each face is a
`rcube::Coordinates2D → Color` map). For more details, see
`rcube::Net` [here](others.md).

### `rcube::BlockArray rcube::Cube::blockRender()`

Returns a 3D representation of the cube, which is pretty similar to the
internal data structures used to store the cube's status. This rendering is
suitable for most applications and is the one used by the official frontend.

A `rcube::BlockArray` is basically an array of 26 `rcube::Blocks`. Each block
contains its position (in `rcube::Coordinates`) and a
`rcube::Orientation → Color` map for its stickers.


### `void rcube::Cube::display()`

Prints the cube to stdout. This output is quite rough (mostly used for
debugging purposes): each face is printed individually as a 3*3 grid of
characters indicating the color (with the corresponging color in the
background).


### `int rcube::Cube::runScript(const std::string &path)`

- `path`: the path to the file;

Runs a Lua script from a file. Returns 1 on success, 0 on failure. Most of the
C++ API has been ported to Lua, for more details about the functions available
[read this](lua.md).


### `int rcube::Cube::runCommand(const std::string &cmd)`

- `cmd`: the code to execute

Runs one line of Lua code. Returns 1 on success, 0 on failure. For more details
about the Lua API [read this](lua.md).