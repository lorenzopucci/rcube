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
#include "misc.hpp"

#define DUMMY_ALGO "LRDUBFLRDUBFLRDUBFLRDUBFLRDUBF"

std::vector<rcube::Move> ph1Moves = {
    rcube::Move('L', 1), rcube::Move('L', 2), rcube::Move('L', -1),
    rcube::Move('R', 1), rcube::Move('R', 2), rcube::Move('R', -1),
    rcube::Move('D', 1), rcube::Move('D', 2), rcube::Move('D', -1),
    rcube::Move('U', 1), rcube::Move('U', 2), rcube::Move('U', -1),
    rcube::Move('B', 1), rcube::Move('B', 2), rcube::Move('B', -1),
    rcube::Move('F', 1), rcube::Move('F', 2), rcube::Move('F', -1)
};

std::vector<rcube::Move> ph2Moves = {
    rcube::Move('D', 1), rcube::Move('D', 2), rcube::Move('D', -1),
    rcube::Move('U', 1), rcube::Move('U', 2), rcube::Move('U', -1),
    rcube::Move('L', 2), rcube::Move('R', 2), rcube::Move('B', 2),
    rcube::Move('F', 2)
};

KociembaSolver::KociembaSolver(const rcube::Cube &cube)
    : _cube(cube), _quick(true), _threads(1), _timeout(60)
{}

KociembaSolver::KociembaSolver(const rcube::Cube &cube, int threads,
    int timeout)
    : _cube(cube), _threads(threads), _timeout(timeout), _quick(false)
{}

void searchPh1 (
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
        prevMovesFaces[0] = Kociemba::getFaceNumber(prevMoves.algorithm
            [prevMoves.algorithm.size() - 1].getAffectedFace());

        if (prevMoves.algorithm.size() > 1)
        {
            prevMovesFaces[1] = Kociemba::getFaceNumber(prevMoves.algorithm
                [prevMoves.algorithm.size() - 2].getAffectedFace());
        }
    }

    for (int i = 0; i < 18; ++i)
    {
        int f = i / 3; // number of the face

        // Pairs of successive moves to the same face can be ignored
        if (f == prevMovesFaces[0]) continue;

        // This avoids some redundancy: between RL2 and L2R only one is executed
        if (f / 2 == prevMovesFaces[0] / 2 && f > prevMovesFaces[0]) continue;

        // This ignores sets of moves like RLR2
        if (f == prevMovesFaces[1] && f / 2 == prevMovesFaces[0] / 2) continue;

        rcube::Move move = ph1Moves[i];

        uint16_t newTwist = Kociemba::twistMove[twist][i];
        uint16_t newFlip = Kociemba::flipMove[flip][i];
        uint16_t newSlice = Kociemba::sliceSortedMove[slice * 24][i] / 24;

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

        if (newDist == 0)
        {
            solutions->push_back(newAlgo);
            return;
        }

        searchPh1(newTwist, newFlip, newSlice, newAlgo, newDist, left - 1, solutions);
    }
}

void searchPh2 (
    uint16_t corners, // current corners: needs to be brought to 0
    uint16_t udEdges, // current udEdges: needs to be brought to 0
    uint16_t sliceSorted, // current sliceSorted: needs to be brought to 0
    rcube::Algorithm prevMoves, // already applied moves for phase 2
    int left, // number of moves after which the maximum length allowed to
              // phase 2 is exceeded
    rcube::Algorithm *shortestSol, // stores the sortest solution found
    int ph1Len, // length of phase 1 solution
    int *shortestLen, // shortest overall solution found (shared between threads)
    long endTime // startTime + timeout
    )
{
    if (prevMoves.length() + ph1Len + 1 >= *shortestLen) return;
    if (time(NULL) > endTime) return;

    int prevMovesFaces[2] = {-2};
    if (prevMoves.algorithm.size() > 0)
    {
        prevMovesFaces[0] = Kociemba::getFaceNumber(prevMoves.algorithm
            [prevMoves.algorithm.size() - 1].getAffectedFace());

        if (prevMoves.algorithm.size() > 1)
        {
            prevMovesFaces[1] = Kociemba::getFaceNumber(prevMoves.algorithm
                [prevMoves.algorithm.size() - 2].getAffectedFace());
        }
    }

    for (int i = 0; i < 10; ++i)
    {
        int f = Kociemba::toPh1Move(i) / 3; // number of the face

        // Pairs of successive moves to the same face can be ignored
        if (f == prevMovesFaces[0]) continue;

        // This avoids some redundancy: between RL2 and L2R only one is executed
        if (f / 2 == prevMovesFaces[0] / 2 && f > prevMovesFaces[0]) continue;

        // This ignores sets of moves like RLR2
        if (f == prevMovesFaces[1] && f / 2 == prevMovesFaces[0] / 2) continue;

        rcube::Move move = ph2Moves[i];

        uint16_t newCorners = Kociemba::cornersMove[corners]
            [Kociemba::toPh1Move(i)];
        uint16_t newUDEdges = Kociemba::udEdgesMove[udEdges][i];
        uint16_t newSliceSorted = Kociemba::sliceSortedMove[sliceSorted]
            [Kociemba::toPh1Move(i)];

        // newDist is only a lower bound for the distance to G1, a sequence of
        // newDist moves to take the cube to G1 does not necessarily exist.
        int newDist = MAX(
            Kociemba::readTable(Kociemba::sliceCornersPrun, N_SLICE_PERM *
                newCorners + newSliceSorted),
            Kociemba::readTable(Kociemba::sliceUdEdgesPrun, N_SLICE_PERM *
                newUDEdges + newSliceSorted)
        );
        if (newDist >= left) continue;

        rcube::Algorithm newAlgo = prevMoves;
        newAlgo.push(move);

        if (newDist == 0)
        {
            if (newAlgo.length() + ph1Len < *shortestLen)
            {
                *shortestSol = newAlgo;
                *shortestLen = newAlgo.length() + ph1Len;
            }
            return;
        }

        searchPh2(newCorners, newUDEdges, newSliceSorted, newAlgo,
            left - 1, shortestSol, ph1Len, shortestLen, endTime);
    }
}

void runPh2Search(Kociemba::CubieCube cc, const rcube::Algorithm &ph1Solution,
    rcube::Algorithm *solution, int *shortestLen, const long &endTime)
{
    *solution = rcube::Algorithm(DUMMY_ALGO);

    int maxDepth = 12;
    while (solution->to_string() == DUMMY_ALGO)
    {
        if (time(NULL) > endTime) return;

        searchPh2(cc.getCorners(), cc.getUDEdges(), cc.getSliceSorted(),
            rcube::Algorithm(), maxDepth, solution, ph1Solution.length(),
            shortestLen, endTime);
        maxDepth++;
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
    long endTime = time(NULL) + _timeout;
    
    int maxDepth = 4;
    while (ph1Solutions.size() < _threads)
    {
        searchPh1(cc.getTwist(), cc.getFlip(), cc.getSliceSorted() / 24,
            rcube::Algorithm(), 0, maxDepth, &ph1Solutions);
        maxDepth++;
    }

    std::sort(ph1Solutions.begin(), ph1Solutions.end(), compareAlgo);

    // quick mode: only run phase 2 on the best solution of phase 1
    if (_quick)
    {
        Kociemba::CubieCube cc1 = cc;
        cc1.performAlgorithm(ph1Solutions[0]);
        rcube::Algorithm ph2Solution;
        int shortestLen = 32;
        
        runPh2Search(cc1, ph1Solutions[0], &ph2Solution, &shortestLen, endTime);
        
        rcube::Algorithm solution = ph1Solutions[0] + ph2Solution;
        solution.normalize();

        std::cout << "[KOCIEMBA] Final solution: " << solution.to_string() <<
            " (" << solution.length() << " moves)\n";

        return solution;
    }

    // slow mode: run phase 2 on the best <_threads> phase 1 solutions in
    // separate threads
    
    rcube::Algorithm ph2Solutions[_threads] = {rcube::Algorithm(DUMMY_ALGO)};
    std::thread ph2Threads[_threads];
    int shortestLen = 32;

    for (int i = 0; i < _threads; ++i)
    {
        Kociemba::CubieCube cc1 = cc;
        cc1.performAlgorithm(ph1Solutions[i]);
        
        std::thread thread(runPh2Search, cc1, ph1Solutions[i],
            ph2Solutions + i, &shortestLen, endTime);

        ph2Threads[i] = move(thread);
    }

    for (int i = 0; i < _threads; ++i)
    {
        if (ph2Threads[i].joinable()) ph2Threads[i].join();
    }
    
    int i = 0;
    for (; i < _threads; ++i)
    {
        if (ph1Solutions[i].length() + ph2Solutions[i].length() == shortestLen)
            break;
    }

    rcube::Algorithm solution = ph1Solutions[i] + ph2Solutions[i];
    solution.normalize();

    std::cout << "[KOCIEMBA] Final solution: " << solution.to_string() <<
        " (" << solution.length() << " moves)\n";

    return solution;
}