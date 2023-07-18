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

extern short twistMove[N_TWIST][N_MOVE];
extern short flipMove[N_FLIP][N_MOVE];
extern short sliceSortedMove[N_SLICE_SORTED][N_MOVE];

extern signed char sliceTwistPrun[N_SLICE * N_TWIST / 2 + 1];
extern signed char sliceFlipPrun[N_SLICE * N_FLIP / 2];

void writeTable(signed char *table, int index, signed char value);

signed char readTable(signed char *table, int index);

void initTables();