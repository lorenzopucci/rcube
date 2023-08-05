/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <rcube.hpp>
#include <utility.hpp>

/*
* This algorithm solves the cube as a human would do. Therefore it does not
* require a lot of computations and runs almost instantly but it is by no
* means optimal as it uses a big number of moves.
*/

class CfopSolver
{
public:
    CfopSolver(const rcube::Cube &cube, Color crossColor = Color::White,
        bool verbose = false);

    rcube::Algorithm cross();
    rcube::Algorithm f2l();
    rcube::Algorithm oll();
    rcube::Algorithm pll();

    rcube::Algorithm solve();

private:
    rcube::Cube _cube;
    bool _verbose;
    Color _crossColor;
};

/*
* This is an implementation of the two phase Kociemba algorithm, which is able
* to solve the cube in less than 25 moves (in average).
* It is possible to trade the solution's length for computing time by adjusting
* some parameters (see below).
*/

class KociembaSolver
{
public:
    // Quick mode: solves the cube almost instantly but the solution is not the
    // shortest possible.
    KociembaSolver(const rcube::Cube &cube);

    // Slow mode: finds a shorter solution but takes more time to run
    // threads: number of phase 1 solutions on which to perform phase 2's search
    // timeout: maximum computing time allowed (in seconds). When it is exceeded,
    // the sortest solution found so far will be returned.
    KociembaSolver(const rcube::Cube &cube, int threads, int timeout);

    // Robot mode will not perform moves on a given face, which can be L,R,B,F
    // but not U and D (designed for robots with only 5 motors). This mode can
    // reduce performances
    void useRobotMode(const rcube::Orientation &face);

    rcube::Algorithm solve();

private:
    rcube::Cube _cube;
    bool _quick;
    int _timeout;
    int _threads;
    int _robotFace = -1;
};