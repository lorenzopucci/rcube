# `rcube::Algorithm`

This class represent an ordered set of [`rcube::Move`s](move.md).

<br>

### `rcube::Algorithm::Algorithm (const std::string& fromString)`

- `fromString`: a standard notation string representing the algorithm;

Constructs the algorithm from a string in the standard notation, that is:
- The letters `R`, `U`, `F`... indicate a move of a single layer (right, up...);
- The letters `r`, `u`, `f`... indicate a joint move of two layers (right, up...);
- The letters `x`, `y` and `z` indicate a rotation of the cube itself around its 3 axes;
- A `'` after a letter inverts the move (counterclockwise instead of clockwise);
- A `2` after a move doubles it (`180°`);
- The characters `()[]`, spaces and tabs are ignored;

This function may throw a `std::invalid_argument` exception for an invalid
string.

<br>

### `rcube::Algorithm::Algorithm (const std::vector<rcube::Move>& fromVector)`

- `fromVector`: a vector of `rcube::Move`s;

Constructs the algorithm from a vector of `rcube::Move`s.

<br>

### `rcube::Algorithm rcube::Algorithm::operator*(const int& factor)`

- `factor`: an integer to multiply the algorithm to;

Multiplies the algorithm by a factor. The algorithm is NOT repeated factor
times, but all the directions of the moves are multiplied by it.

Note: only designed and tested for `factor = {-1, 1, 2}`

<br>

### `rcube::Algorithm rcube::Algorithm::operator+(const rcube::Algorithm a)`

- `a`: the algorithm to concatenate;

Adds up two algorithms by concatenating them.

<br>

### `inline void rcube::Algorithm::push(const rcube::Move &m)`

- `m`: the move to push;

Pushes a `rcube::Move` to the end of the algorithm.

<br>

### `static rcube::Algorithm rcube::Algorithm::generateScramble(const int &length = 12)`

- `length`: the lenght of the scramble (number of moves);

Returns a randomly generated algorithm of given length.

<br>

### `void rcube::Algorithm::normalize()`

Rewrites the algorithm by merging consecutive moves of the same layer
(e.g. `UU → U2`)

<br>

### `rcube::Algorithm rcube::Algorithm::reverse() const`

Returns the inverse algorithm, i.e. the algorithm obtained by reading the initial
algorithm right-to-left and inverting the moves' directions. If applied
after the uninverted algorithm, this new algorithm will get the cube back to its
initial state.

<br>

### `rcube::Algorithm rcube::Algorithm::withoutRotations() const`

Returns an algorithm equivalent to the current instance but without cube's
rotations (`x`, `y`, `z`). For example, `RyRF'` become `RBR'`.

<br>

### `void rcube::Algorithm::removeRotations()`

Same as `withoutRotations`, but instead of being returned, the new algorithm is
applied to the current instance

<br>

### `std::string rcube::Algorithm::to_string() const`

Returns a string representing the algorithm in the standard notation.

<br>

### `inline int rcube::Algorithm::length() const`

Returns the length of the algorithm (number of moves).

<br>