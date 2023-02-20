# Lua integration

Most of the C++ API is also available for Lua. To run some Lua code, use the
following functions:
- [`int rcube::Cube::runScript(const std::string &path)`](cube.md#int-rcubecuberunscriptconst-stdstring-path)
- [`int rcube::Cube::runCommand(const std::string &cmd)`](cube.md#int-rcubecuberuncommandconst-stdstring-cmd)

This file contains a list of all the Lua functions available.

<br>

### `<string> scramble(<number> length)`

- `length`: the random algorithm's length;

Equivalent to `rcube::Cube::scramble`. The generated algorithm is returned as
a string.

<br>

### `performMove(<string> move, <number> direction)`

- `move`: the move to perform (`U`, `R`, `M`, `x`...);
- `direction`: `+1` for clockwise, `-1` for counterclockwise, `2` for double;

Equivalent to `rcube::Cube::performMove`. Does not return anything.

<br>

### `performAlgorithm(<string> algorithm)`

- `algorithm`: the algorithm to perform;

Equivalent to `rcube::Cube::performAlgorithm`. Does not return anything.

<br>

### `<boolean> isSolved()`

Equivalent to `rcube::Cube::isSolved`. Returns `true` when the cube is solved.

<br>

### `rotateTo(<string> topColor, <string> frontColor)`

- `topColor`: the color that the top center will have
- `frontColor`: the color that the front center will have

Equivalent to `rcube::Cube::rotateTo`.

<br>

### `<table> find(<string> stickers)`

- `stickers`: string containing a character for each sticker in the block to
find (e.g. `gw` for the green-white edge);

Equivalent to `rcube::Cube::find`. The coordinates are returned in a table:
`{x=#, y=#, z=#}`.

<br>

### `<table> getStickerOrientation(<table> position, <string> color)`

- `position`: the coordinates of the block to find (`{x=#, y=#, z=#}`);
- `color`: the color of the sticker to find;

Equivalent to `rcube::Cube::getStickerOrientation`. The orientation is returned
in a table: `{axis=#, direction=#}`.

<br>

### `<string> getFaceColor(<table> face)`

- `face`: the face (`{axis=#, direction=#}`);

Equivalent to `rcube::Cube::getFaceColor`. The color is returned as a
one-character string (e.g. `"w"` for white).

<br>

### `<table> getFaceOrientation(<string> color)`

- `color`: the color of the face;

Equivalent to `rcube::Cube::getFaceOrientation`. The orientation is returned
in a table.

<br>

### `<string> getStickerAt(<table> pos, <table> orient)`

- `pos`: the coordinates of the block the sticker belongs to (`{x=#, y=#, z=#}`);
- `orient`: the sticker's orientation (`{axis=#, direction=#}`);

Equivalent to `rcube::Cube::getStickerAt`. The color is returned as a
one-character string (e.g. `"w"` for white).

<br>

### `display()`

Equivalent to `rcube::Cube::display`.

<br>

### `<boolean> faceMatches(<table> orientation, <string> expr, [<table> dest])`

- `orientation`: the orientation of the face to check (`{axis=#, direction=#}`);
- `expr`: the regular expression to compare with the face. For its syntax
[read here](cube.md#rcubecubefacematchesconst-rcubeorientation-face-const-stdstring-expr)
- `dest`: the place to which the block corresponding to the first character of
`expr` will be rotated (optional);

Equivalent to `rcube::Cube::faceMatches`.

<br>

### `<boolean> layerMatches(<table> orientation, <string> expr, [<table> dest, <table> orient])`

- `orientation`: the orientation of the layer to check (`{axis=#, direction=#}`);
- `expr`: the regular expression to compare with the layer. For its syntax
[read here](cube.md#rcubecubefacematchesconst-rcubeorientation-face-const-stdstring-expr)
- `dest`: the place to which the block corresponding to the first character of
`expr` will be rotated (optional);
- `orient`: the orientation at which the sticker corresponding to the first
character of `expr` will be placed;

Equivalent to `rcube::Cube::layerMatches`

<br>

### `<boolean> layerAndFaceMatch(<table> orientation, <string> expr, [<table> dest, <table> orient])`

- `orientation`: the orientation of the layer to check (`{axis=#, direction=#}`);
- `expr`: the regular expression. For its syntax
[read here](cube.md#rcubecubefacematchesconst-rcubeorientation-face-const-stdstring-expr)
- `dest`: the place to which the block corresponding to the first character of
`expr` will be rotated (optional);
- `orient`: the orientation at which the sticker corresponding to the first
character of `expr` will be placed;

Equivalent to `rcube::Cube::layerAndFaceMatch`

<br>

### `<string> normalizeAlgo(<string> algo)`

- `algo`: the algorithm to normalize;

Equivalent to `rcube::Algorithm::normalize`. The normalized algorithm is
returned as a string.

<br>

### `<string> reverseAlgo(<string> algo)`

- `algo`: the algorithm to reverse;

Equivalent to `rcube::Algorithm::reverse`. The reversed algorithm is
returned as a string.

<br>

### `<string> removeRotations(<string> algo)`

- `algo`: the algorithm to remove rotations from;

Equivalent to `rcube::Algorithm::removeRotations`. The new algorithm is
returned as a string.

<br>

### `<string> solveCfop()`

Equivalent to `rcube::Cube::solveCfop`. The algorithm used to solve the cube is
returned as a string.
