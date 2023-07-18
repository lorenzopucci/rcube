/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <solving.hpp>

#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

#include "cubieCube.hpp"
#include "lookupTables.hpp"


#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))


std::vector<rcube::Move> ph1Moves = {
    rcube::Move('L', 1), rcube::Move('L', 2), rcube::Move('L', -1),
    rcube::Move('R', 1), rcube::Move('R', 2), rcube::Move('R', -1),
    rcube::Move('D', 1), rcube::Move('D', 2), rcube::Move('D', -1),
    rcube::Move('U', 1), rcube::Move('U', 2), rcube::Move('U', -1),
    rcube::Move('B', 1), rcube::Move('B', 2), rcube::Move('B', -1),
    rcube::Move('F', 1), rcube::Move('F', 2), rcube::Move('F', -1)
};

std::vector<rcube::Move> ph2Moves = {
    rcube::Move('U', 1), rcube::Move('U', -1), rcube::Move('U', 2),
    rcube::Move('D', 1), rcube::Move('D', -1), rcube::Move('D', 2),
    rcube::Move('R', 2), rcube::Move('L', 2), rcube::Move('F', 2),
    rcube::Move('B', 2)
};

KociembaSolver::KociembaSolver(const rcube::Cube &cube, bool verbose)
    : _cube(cube), _verbose(verbose)
{}


void search (
    uint16_t twist, uint16_t flip, uint16_t slice,
    rcube::Algorithm prevMoves,
    int dist, int left
    )
{
    rcube::Orientation prevMoveOrient = {Axis::X, 0};
    if (prevMoves.algorithm.size() > 0)
    {
        prevMoveOrient = prevMoves.algorithm[prevMoves.algorithm.size() - 1]
            .getAffectedFace();
    }

    for (int i = 0; i < 18; ++i)
    {
        rcube::Move move = ph1Moves[i];
        if (move.getAffectedFace() == prevMoveOrient) continue;

        uint16_t newTwist = twistMove[twist][i];
        uint16_t newFlip = flipMove[flip][i];
        uint16_t newSlice = sliceSortedMove[slice * 24][i] / 24;

        int newDist = MAX(
            readTable(sliceFlipPrun, N_SLICE * newFlip + newSlice),
            readTable(sliceTwistPrun, N_SLICE * newTwist + newSlice)
        );
        if (newDist >= left) continue;

        rcube::Algorithm newAlgo = prevMoves;
        newAlgo.push(move);

        if (newDist == 0) std::cout << "[KOCIEMBA] phase 1: " << newAlgo.to_string() << std::endl;

        search(newTwist, newFlip, newSlice, newAlgo, newDist, left - 1);
    }
}

rcube::Algorithm KociembaSolver::solve()
{
    initTables();
    Kociemba::CubieCube cc(_cube);
    
    search(cc.getTwist(), cc.getFlip(), cc.getSliceSorted() / 24,
        rcube::Algorithm(), 0, 12);

    return rcube::Algorithm();
}