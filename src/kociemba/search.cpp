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
#include <algorithm>

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

inline int getFaceNumber(const rcube::Orientation &orient)
{
    // associates a number from 0 to 5 to each face (the order is L, R, D, U,
    // B, F)

    return (int)orient.axis * 2 + ((orient.direction + 1) / 2);
}

void search (
    uint16_t twist, // current twist: needs to be brought to 0
    uint16_t flip, // current flip: needs to be brought to 0
    uint16_t slice, // current slice: needs to be brought to 0
    rcube::Algorithm prevMoves, // already applied moves for phase 1
    int dist, // lower bound for the number of moves needed to reach G1
    int left, // number of moves after which the maximum length allowed to
              // phase 1 is exceeded
    std::vector<rcube::Algorithm> *solutions // stores all the solutions found
    )
{
    int prevMovesFaces[2] = {-2};
    if (prevMoves.algorithm.size() > 0)
    {
        prevMovesFaces[0] = getFaceNumber(prevMoves.algorithm
            [prevMoves.algorithm.size() - 1].getAffectedFace());

        if (prevMoves.algorithm.size() > 1)
        {
            prevMovesFaces[1] = getFaceNumber(prevMoves.algorithm
                [prevMoves.algorithm.size() - 2].getAffectedFace());
        }
    }

    for (int i = 0; i < 18; ++i)
    {
        int f = i / 6; // number of the face

        // Pairs of successive moves to the same face can be ignored
        if (f == prevMovesFaces[0]) continue;

        // This avoids some redundancy: between RL2 and L2R only one is executed
        if (f / 2 == prevMovesFaces[0] / 2 && f > prevMovesFaces[0]) continue;

        // This ignores sets of moves like RLR2
        if (f == prevMovesFaces[1] && f / 2 == prevMovesFaces[0] / 2) continue;

        rcube::Move move = ph1Moves[i];

        uint16_t newTwist = Kociemba::twistMove[twist][i];
        uint16_t newFlip = Kociemba::flipMove[flip][i];
        uint16_t newSlice = Kociemba::yEdgesMove[slice * 24][i] / 24;

        // newDist is only a lower bound for the distance to G1, a sequence of
        // newDist moves to take the cube to G1 does not necessarily exist.
        int newDist = MAX(
            Kociemba::readTable(Kociemba::sliceFlipPrun, N_SLICE *
                newFlip + newSlice),
            Kociemba::readTable(Kociemba::sliceTwistPrun, N_SLICE *
                newTwist + newSlice)
        );
        if (newDist >= left) continue;

        rcube::Algorithm newAlgo = prevMoves;
        newAlgo.push(move);

        if (newDist == 0) solutions->push_back(newAlgo);

        search(newTwist, newFlip, newSlice, newAlgo, newDist, left - 1, solutions);
    }
}

bool compareAlgo(rcube::Algorithm a, rcube::Algorithm b)
{
    return a.length() < b.length();
}

rcube::Algorithm KociembaSolver::solve()
{
    Kociemba::initTables();
    Kociemba::CubieCube cc(_cube);
    std::vector<rcube::Algorithm> ph1Solutions;
    
    search(cc.getTwist(), cc.getFlip(), cc.getYEdges() / 24,
        rcube::Algorithm(), 0, 10, &ph1Solutions);

    std::cout << "Found " << ph1Solutions.size() << " solutions\n";

    std::sort(ph1Solutions.begin(), ph1Solutions.end(), compareAlgo);

    for (int i = 0; i < MIN(20, ph1Solutions.size()); ++i)
    {
        std::cout << ph1Solutions[i].length() << "- " << ph1Solutions[i].to_string() << std::endl;
    }

    return rcube::Algorithm();
}