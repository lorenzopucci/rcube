# `rcube::Move`

This class represent a single twist of a cube's face.

<br>

### `rcube::Move::Move(MoveFace face, MoveDirection direction)`

- `face`: the face involved by the move (see `MoveFace` in `include/utility.hpp`);
- `direction` the direction of the move (see `MoveDirection` in `include/utility.hpp`);

Initializes the move from a face and a direction given as enums.

<br>

### `rcube::Move::Move(char face, int direction)`

- `face`: the face involved by the move (`R`, `B`, `M`...);
- `direction` the direction of the move (`1`, `2`, `-1`);

Initializes the move from a face and a direction given as a char and int.

<br>

### `rcube::Move::Move(const rcube::Orientation &orient, int direction)`

- `orient`: the face involved by the move (see `rcube::Orientation`);
- `direction` the direction of the move (`1`, `2`, `-1`);

Initializes the move from a face and a direction given as a `rcube::Orient`
and an int.

<br>

### `std::string rcube::Move::std::string to_string()`

Returns a string representing the move in standard notation (e.g. `R`, `U'`, `B2`).

<br>

### `rcube::Orientation rcube::Move::getAffectedFace()`

Returns the face the move acts on as a `rcube::Orientation`
(e.g. `{Axis::X, -1}` for `L`).

<br>

### `rcube::Move rcube::Move::getInverted()`

Returns a move applied to the same face but in the opposite direction
(e.g. `R` for `R'` and `D2` for `D2`).