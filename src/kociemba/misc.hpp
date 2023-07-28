/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <rcube.hpp>

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))

namespace Kociemba
{

// associates a number from 0 to 5 to each face (the order is L, R, D, U, B, F)
int getFaceNumber(const rcube::Orientation &orient);

// Converts an index of ph1Moves (see search.cpp) to an index of ph2Moves
int toPh2Move(int ph1Move);

// Converts an index of ph2Moves (see search.cpp) to an index of ph1Moves
int toPh1Move(int ph2Move);

// Compute binomial coefficients
int choose(int n, int k);

}; // namespace Kociemba