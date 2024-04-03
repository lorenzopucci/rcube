# Other data structures in `rcube`

## `rcube::Orientation`

This struct represents a pair of (axis, sign), which uniquely identifies each
face of the cube (e.g. the bottom face is (`Axis::Y`, `-1`)).

<br>

### `bool rcube::Orientation::operator<(const rcube::Orientation& a)`

Orientation `A` is < orientation `B` if `A.axis < B.axis` (where
`X=0, Y=1, Z=2`) or `A.direction < B.direction` in case the two axes are equal.

<br>

### `bool rcube::Orientation::operator==(const rcube::Orientation& a)`

Returns true when both the axes and the directions are equal.

<br>

### `bool rcube::Orientation::operator!=(const rcube::Orientation& a)`

Returns true when the axes or the directions are different.

<br>

### `void rcube::Orientation::rotate(const Axis& axis, int step)`

- `axis`: the axis of rotation;
- `step`: the number of 90° rotations to apply (clockwise relatively to someone
    looking at the origin from the positive side of the axis);

Rotates the orientation around an axis of a given number of steps.

<br>

### `rcube::Orientation rcube::Orientation::getRotated(const Axis& axis, int step)`

- `axis`: the axis of rotation;
- `step`: the number of 90° rotations to apply (clockwise relatively to someone
    looking at the origin from the positive side of the axis);

Same as `rcube::Orientation::rotate` but the new orientation is returned and
does not overwrite the previous one.

<br>

### `rcube::Orientation rcube::Orientation::getInv()`

Returns an orientation with the same axis but opposite direction.

<br>

### `static std::vector<rcube::Orientation> rcube::Orientation::iterate()`

Returns a vector containing all 6 possible orientations in the following
order: `-X`, `+X`, `-Y`, `+Y`, `-Z`, `+Z`.

<br>

### `static std::vector<rcube::Orientation> rcube::Orientation::iterateOnBlock(const rcube::Coordinates &block)`

- `block`: the coordinates of a cube's block

Returns a vector containing all the 2 (or 3) orientations of the stickers of an
edge (or corner) of given coordinates.

<br><br>

## `rcube::Coordinates`

This class represents a set of cartesian coordinates in 3D space. This is
specifically designed for the coordinates of a `3*3` cube's blocks, so the
values of each coordinates are generally in the set `{-1, 0, +1}`.

<br>

### `rcube::Coordinates::Coordinates(const int& x, const int& y, const int& z)`

- `x`: X component;
- `y`: Y component;
- `z`: Z component;

The values of `x,y,z` are directly assigned from the arguments.

<br>

### `rcube::Coordinates::Coordinates(const rcube::Orientation& o1)`

- `o1`: the orientation of a cube's face;

Initializes the class with the coordinates of the center belonging to the
face `o1`.

<br>

### `rcube::Coordinates::Coordinates(const rcube::Orientation& o1, const rcube::Orientation& o2)`

- `o1`: the orientation of a cube's face;
- `o2`: the orientation of a cube's face;

Initializes the class with the coordinates of the edge between the two faces
`o1` and `o2`.

<br>

### `rcube::Coordinates::Coordinates(const rcube::Orientation& o1, const rcube::Orientation& o2, const rcube::Orientation& o3)`

- `o1`: the orientation of a cube's face;
- `o2`: the orientation of a cube's face;
- `o3`: the orientation of a cube's face;

Initializes the class with the coordinates of the corner between the faces
`o1`, `o2` and `o3`.

<br>

### `int rcube::Coordinates::x()`, `int rcube::Coordinates::y()`, `int rcube::Coordinates::z()`

These functions return the `X`, `Y` and `Z` component of the coordinates
respectively.

<br>

### `bool rcube::Coordinates::operator==(const rcube::Coordinates &a)`

Returns true when all 3 components are equal.

<br>

### `bool rcube::Coordinates::operator!=(const rcube::Coordinates &a)`

Returns true when the two coordinates differ in at least 1 component.

<br>

### `std::string rcube::Coordinates::to_string()`

Returns a string of the form `(x, y, z)` representing the coordinates.

<br>

### `void rcube::Coordinates::rotate(const Axis& axis, int step)`

- `axis`: the axis of rotation;
- `step`: the number of 90° rotations to apply (clockwise relatively to someone
    looking at the origin from the positive side of the axis);

Rotates the coordiantes around an axis of a given number of steps.

<br>

### `rcube::Coordinates getRotated(const Axis& axis, int step)`

- `axis`: the axis of rotation;
- `step`: the number of 90° rotations to apply (clockwise relatively to someone
    looking at the origin from the positive side of the axis);

Same as `rcube::Coordinates::rotate` but the new coordinates are returned and
do not overwrite the previous ones.

<br>

### `static std::vector<Coordinates> iterateCorners()`

Returns a vector containing the 8 coordinates of the corners.

<br>

### `static std::vector<Coordinates> iterateEdges()`

Returns a vector containing the 12 coordinates of the edges.

<br><br>


## `rcube::Coordinates2D`

This struct represents a set of cartesian coordinates in 2D space namely
the coordinates of a sticker relatively to its face. The
values of each coordinates are generally in the set `{-1, 0, +1}`.

<br>

### `rcube::Coordinates2D::Coordinates2D(const int& x, const int& y)`

- `x`: X component;
- `y`: Y component;

The values of `x,y` are directly assigned from the arguments.

<br>

### `rcube::Coordinates2D::Coordinates2D(const rcube::Coordinates& coords3D, const rcube::Orientation& o)`

- `coords3D`: some 3D coordinates;
- `o`: the orientation of a sticker of a block at `coords3D`

The resulting coordinates are the ones of the sticker with orientation
`o` belonging to the block at `coords3D`.

The output of this function is quite random i.e. the position of
the 2D axes in the face is determined but may not be the most intuitive.

<br>

### `int rcube::Coordinates::x()`, `int rcube::Coordinates::y()`

These functions return the `X` and `Y` component of the coordinates
respectively.

<br>

### `bool rcube::Coordinates::operator<(const rcube::Coordinates &a)`

Coordinates A are < coordinates B when `A.x < B.x` or when `A.x == B.x` and
`A.y < B.y`.

<br>

### `bool rcube::Coordinates::operator==(const rcube::Coordinates &a)`

Returns true when both components are equal.

<br>

### `void rcube::Coordinates::rotate(int step)`

- `step`: the number of 90° rotations to apply (positive = anticlockwise);

Rotates the coordiantes around `(0,0)` of a given number of steps.