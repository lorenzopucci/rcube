# `rcube::Cube`
This class represents a $3\times3\times3$ Rubik's cube.

<br>

### `rcube::Cube::Cube(const Color& topColor = Color::White, const Color& frontColor = Color::Green)`

- `topColor`: the color of the top face (default: `Color::White`)
- `frontColor`: the color of the front face (default: `Color::Green`)

This is the default constructor. It initializes a solved cube with the
orientation specified by `topColor` and `frontColor`.

NOTE: this function may throw a `std::invalid_argument` exception if `topColor`
and `frontColor` are not two adjacent colors.

<br>

### `rcube::Cube::Cube(const std::map<rcube::Orientation, rcube::MixedFace> &data)`

- `data`: the data used to initialize the cube. It is an array of
`rcube::MixedFace` mapped to their orientation.

It initializes a cube (solved or scrambled) from an array of faces, each
containing an array of stickers. See `rcube::MixedFace` for more details.

NOTE: this function may throw a `std::invalid_argument` exception if the input
is not vaild or does not represent a solvable cube.

<br>

### `void rcube::Cube::performMove(const rcube::Move &move)`

- `move`: the move to perform;

Applies a move to the cube. A move can be the rotation of a layer or the
rotation of the cube itself (change of viewpoint).

<br>

### `void rcube::Cube::performAlgorithm (const rcube::Algorithm& algorithm)`

- `algorithm`: the algorithm to perform (see [`rcube::Algorithm`](algorithm.md));

Applies an algorithm (set of moves) to the cube.

<br>

### `void rcube::Cube::performAlgorithm (const std::string& algorithm)`

- `algorithm`: the algorithm to perform as a string;

Shortcut for `performAlgorithm(rcube::Algorithm("string"))`

<br>

### `void rcube::Cube::scramble(const int &length = 12, rcube::Algorithm* dest = nullptr)`

- `length`: the scramble's length (number of random moves, default: `12`);
- `dest`: a pointer to an algorithm where the generated algorithm will be
stored (default: `nullptr`);

Applies a randomly generated algorithm of the given size. The algorithm can be
retrieved by specifying a valid pointer.

<br>

### `bool rcube::Cube::isSolved()`
Returns `true` when the cube is solved (all the blocks are in the right place).

<br>

### `bool rcube::Cube::isSolvable()`

Returns `true` when the current status of the cube can be obtained by applying
moves to a solved cube.

A cube is not considered solvable when:
- There are more or less than 9 stickers for each color
- There are invalid or double blocks (like the corner `RGO`)
- The centers are not placed correctly

<br>

### `void rcube::Cube::rotateTo(const Color& topColor, const Color& frontColor)`

- `topColor`: the color that the top center will have;
- `frontColor`: the color that the front center will have;

Rotates the cube until `topColor` and `frontColor` become the colors of the
top and front center respectively.

NOTE: this function may throw a `std::invalid_argument` exception if `topColor`
and `frontColor` are not two adjacent colors.

<br>

### `rcube::Coordinates rcube::Cube::find(const Color& c1)`

- `c1`: the color of the center to find;

Returns the coordinates of a center given its color.

<br>

### `rcube::Coordinates rcube::Cube::find(const Color& c1, const Color &c2)`

- `c1` and `c2`: the colors of the stickers belonging to the edge to find;

Returns the coordinates of an edge given the colors of its stickers.

NOTE: this function may throw a `std::invalid_argument` exception if the two
colors are not adjacent.

<br>

### `rcube::Coordinates rcube::Cube::find(const Color& c1, const Color &c2, const Color &c3)`

- `c1`, `c2` and `c3`: the colors of the stickers belonging to the corner to
find;

Returns the coordinates of a corner given the colors of its stickers.

NOTE: this function may throw a `std::invalid_argument` exception if the three
colors cannot form a corner.

<br>

### `rcube::Orientation rcube::Cube::getStickerOrientation(const rcube::Coordinates &coords, const Color &color)`

- `coords`: the coordinates of the block the sticker belongs to;
- `color`: the sticker's color;

Returns the orientation of the sticker of a given color belonging to the block
at some given coordinates.
 
NOTE: this function may throw a `std::invalid_argument` exception if there is
no sticker of such color in the block or if the coordinates are not valid.

<br>

### `Color rcube::Cube::getFaceColor(const rcube::Orientation &face)`

- `face`: the face (as a `rcube::Orientation`);

Returns the color of the center of a given face.

<br>

### `Color rcube::Cube::getFaceOrientation(const Color &color)`

- `face`: the color of the face;

Returns the orientation of a face given the color of its center.

<br>

### `Color rcube::Cube::getStickerAt(const rcube::Coordinates &pos, const rcube::Orientation &orient)`

- `pos`: the coordinates of the block the sticker belongs to;
- `orient`: the sticker's orientation;

Returns the color of the sticker at some given coordinates, with a given
orientation.

NOTE: this function may throw a `std::invalid_argument` exception if there is
no such sticker in the block or if the coordinates are not valid.

<br>

### `rcube::Cube::faceMatches(const rcube::Orientation &face, const std::string &expr, const rcube::Coordinates &dest = rcube::Coordinates(0,0,0), rcube::Algorithm *algo = nullptr)`

- `face`: the face to check (expressed with its orientation);
- `expr`: the regualr expression;
- `dest`: the place to which the block corresponding to the first character of
`expr` will be rotated (see below);
- `algo`: a pointer to a `rcube::Algorithm` where the possible moves involved in
adjusting the face will be stored;

Returns `true` when the stickers of the given face match the given pattern.
The pattern is a `std::string` with the following syntax:

- Every star (`*`) indicates a sticker that can be of any color;
- The uppercase letters `W`, `Y`, `G`, `B`, `R`, `O` indicate a sticker of the
corresponding color (`W` for white, `Y` for yellow and so on);
- The lowercase letters `w`, `y`, `g`, `b`, `r`, `o` indicate a sticker that
is NOT of the corresponding color (`w` is every color but white);
- All the letters from `A` to `L` that are not colors indicate a sticker
of unspecified color, but with the following rules:
    - For all pairs of uppercase and lowercase letters, the first case used in
    the expression picks a color, and the letter of the other case corresponds
    to all colors but that one (if `A`, which appears first in the string, is
    green, `a` is all colors but green);
    - Different letters may not correspond to different colors;
- All the letters from `M` to `Z` that are not colors also indicate a sticker
of an unspecified color, but with the following rules:
    - Different letters correspond to different colors;
    - A pair of uppercase and lowercase letters correspond to opposite colors
    (if `M` is blue, `m` is green);

The face is read left-to-right, top-to-bottom:
```
             123
123456789 -> 456
             789
```
For example, a cross is `*A*AAA*A*`

Note: the string MUST be 9 characters long, otherwise it will not be accepted.

If a match is found and `dest` is not `(0,0,0)`, the face will be rotated
until the block corresponding to the first character of `expr` ends up at the
given position. If `algo` is not `nullptr`, the moves used to do so will be
stored there.

<br>

### `rcube::Cube::layerMatches(const rcube::Orientation &layer, const std::string &expr, const rcube::Coordinates &dest = rcube::Coordinates(0,0,0), const rcube::Orientation &orient = {Axis::X, 0}, rcube::Algorithm *algo = nullptr)`

- `layer`: the layer to check (expressed with the orientation of its adjacent
face. For middle layer, use `layer.direction=0` even though
`rcube::Orientation` is not designed for that value);
- `expr`: the regular expression;
- `dest`: the place to which the block corresponding to the first character of
`expr` will be rotated (see below);
- `orient`: determines the position of the first sticker along with `dest`;
- `algo`: a pointer to a `rcube::Algorithm` where the possible moves involved in
adjusting the face will be stored;

Returns `true` when the stickers of the given layer match the given pattern.
The term layer means the set of stickers adjacent to the same face or one of
the middle layers (the ones to which the moves `M`, `S` and `E` are applied).

The regualr expession's syntax is the same as in `rcube::cube::faceMatches`
but instead of 9, it must be 12 characters long.

If a match is found and `dest` is not `(0,0,0)`, the layer will be rotated
until the block corresponding to the first character of `expr` ends up at the
given position. If `orient` is not `{X, 0}`, not only will the block be in the
right place, but the right sticker will also be at the given orientation. If
`algo` is not `nullptr`, the moves used to do so will be stored there.

<br>

### `rcube::Cube::layerAndFaceMatch(const rcube::Orientation &layer, const std::string &expr, const rcube::Coordinates &dest = rcube::Coordinates(0,0,0), const rcube::Orientation &orient = {Axis::X, 0}, rcube::Algorithm *algo = nullptr)`

- `layer`: the layer to check;
- `expr`: the regular expression;
- `dest`: the place to which the block corresponding to the first character of
`expr` will be rotated (see below);
- `orient`: determines the position of the first sticker along with `dest`;
- `algo`: a pointer to a `rcube::Algorithm` where the possible moves involved in
adjusting the face will be stored;

Returns `true` when both the face and its adjacent stickers match a given
regular expression (as in `rcube::cube::faceMatches`).

The expression must have the following structure:
```
<face expr (9 char)>-<layer expr (12 char)>
```
Where the first stickers of both expressions belong to the same block and the
first 3 stickers of the layer are adjacent to the first 3 stickers of the face.

If a match is found and `dest` is not `(0,0,0)`, the layer will be rotated
until the block corresponding to the first character of `expr` ends up at the
given position. If `orient` is not `{X, 0}`, not only will the block be in the
right place, but the right sticker will also be at the given orientation. If
`algo` is not `nullptr`, the moves used to do so will be stored there.

<br>

### `rcube::Net rcube::Cube::netRender()`

Returns a 2D representation of the cube (basically a
`rcube::Orientation → rcube::Face` map, where each face is a
`rcube::Coordinates2D → Color` map).

<br>

### `rcube::BlockArray rcube::Cube::blockRender()`

Returns a 3D representation of the cube, which is pretty similar to the
internal data structures used to store the cube's status. This rendering is
suitable for most applications and is the one used by the official frontend.

A `rcube::BlockArray` is basically an array of 26 `rcube::Blocks`. Each block
contains its position (in `rcube::Coordinates`) and a
`rcube::Orientation → Color` map for its stickers.

<br>

### `void rcube::Cube::display()`

Prints the cube to stdout. This output is quite rough (mostly used for
debugging purposes): each face is printed individually as a 3*3 grid of
characters indicating the color (with the corresponging color in the
background).

<br>

### `int rcube::Cube::runScript(const std::string &path)`

- `path`: the path to the file;

Runs a Lua script from a file. Returns 1 on success, 0 on failure. Most of the
C++ API has been ported to Lua, for more details about the functions available
[read this](lua.md).

<br>

### `int rcube::Cube::runCommand(const std::string &cmd)`

- `cmd`: the code to execute

Runs one line of Lua code. Returns 1 on success, 0 on failure. For more details
about the Lua API [read this](lua.md).

<br>

### `rcube::Algorithm solveCfop(bool verbose = false, Color crossColor = Color::White)`

- `verbose`: enable output to stdout;
- `crossColor`: the color of the face to start from

Initializes a `CfopSolver` with the given arguments and solves the cube. Returns
the algorithm used.