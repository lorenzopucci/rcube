/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <string>
#include <cstdint>

#define N_FLIP 2048
#define N_TWIST 2187
#define N_SLICE 495
#define N_SLICE_SORTED 11880
#define N_MOVE 18

#define STD_PATH "kociemba_lookup_tables"

namespace Kociemba
{

// In the tables below, each of the 18 moves is referenced with a number. The
// order is the following one: L, L2, L', R, R2, R', D, D2, D', U, U2, U',
// B, B2, B', F, F2, F'

// Stores the effect of the 18 moves on all the 2187 different values of twist
// (twistMove[twist][M] = new twist after applying M).
// The corresponding file size is 78.7kB
extern short twistMove[N_TWIST][N_MOVE];

// Stores the effect of the 18 moves on all the 2048 different values of flip
// (flipMove[flip][M] = new flip after applying M).
// The corresponding file size is 73.7kB
extern short flipMove[N_FLIP][N_MOVE];

// Stores the effect of the 18 moves on all the 11880 different values of
// sliceSorted (sliceSortedMove[sliceSorted][M] = new sliceSorted after applying
// M).
// The corresponding file size is 427.7kB
extern short sliceSortedMove[N_SLICE_SORTED][N_MOVE];

// Stores the number of moves required to reach twist=0 and slice=0 from all the
// 2187*495 possible combinations of twist and slice values. This is one of the
// two pruning tables used in phase 1.
// The corresponding file size is 541.3kB
extern signed char sliceTwistPrun[N_SLICE * N_TWIST / 2 + 1];

// Stores the number of moves required to reach flip=0 and slice=0 from all the
// 2048*495 possible combinations of flip and slice values. This is the second
// pruning table used in phase 1.
// The corresponding file size is 506.9kB
extern signed char sliceFlipPrun[N_SLICE * N_FLIP / 2];

// Since all the values of sliceTwistPrun and sliceFlipPrun are < 16 (actually
// they are all <=9), to save space in memory, each byte (signed char) contains
// two of them. To handle this, the tables are not read and written directly but
// the functions writeTable and readTable need to be used (see below).

// Write a value to a given index of a table (namely sliceTwistPrun or
// sliceFlipPrun). Note that index can go up to twice as much as the tables are
// declared with (because each byte stores two values).
void writeTable(signed char *table, int index, signed char value);


// Read the value at a given index of a table (namely sliceTwistPrun or
// sliceFlipPrun). Note that index can go up to twice as much as the tables are
// declared with (because each byte stores two values).
signed char readTable(signed char *table, int index);

// Load the tables from the corresponding files or regenerate them if the files
// do not exist. This function requires CLI interaction with the user.
void initTables();

} // namespace Kociemba