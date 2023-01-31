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