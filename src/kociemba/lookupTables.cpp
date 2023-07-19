/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <vector>
#include <iostream>

#include "cubieCube.hpp"
#include "fileManager.hpp"
#include "lookupTables.hpp"
#include <rcube.hpp>

namespace Kociemba
{

short twistMove[N_TWIST][N_MOVE] = {{0}};
short flipMove[N_FLIP][N_MOVE] = {{0}};
short sliceSortedMove[N_SLICE_SORTED][N_MOVE] = {{0}};

signed char sliceTwistPrun[N_SLICE * N_TWIST / 2 + 1] = {0};
signed char sliceFlipPrun[N_SLICE * N_FLIP / 2] = {0};


signed char readTable(signed char *table, int index)
{
    // The leftmost 4 bits of each byte are used to store the values at odd
    // indices, the other 4 bits are used to store the values at even indices.
    // Therefore the table in base 2 looks like {BBBBAAAA, DDDDCCCC, ...} where
    // AAAA is the value at index 0, BBBB is the value at index 1 and so on.

    if (index % 2 == 0)
        return table[index / 2] & 0x0f;
    
    return (table[index / 2] >> 4) & 0x0f;
}

void writeTable(signed char *table, int index, signed char value)
{
    if (index % 2 == 0)
        table[index / 2] &= 0xf0 | value;
    else
        table[index / 2] &= 0x0f | (value << 4);
}

void generateTwistMove(const std::string &path)
{
    std::cout << "[KOCIEMBA] Generating twistMove table...\n";

    CubieCube cube = CubieCube();
    auto orients = rcube::Orientation::iterate();

    for (int i = 0; i < N_TWIST; ++i)
    {
        cube.setTwist(i);

        for (int k = 0; k < 6; ++k)
        {
            rcube::Move mv(orients[k], 1);

            for (int j = 0; j < 3; ++j)
            {
                cube.cornerMultiply(mv);
                twistMove[i][3 * k + j] = cube.getTwist();
            }
            cube.cornerMultiply(mv); // resets the cube's state
        }
    }
    writeFile((char*)twistMove, sizeof(twistMove), "twistMove", path);

    std::cout << "[KOCIEMBA] Generated twistMove table\n";
}

void generateFlipMove(const std::string &path)
{
    std::cout << "[KOCIEMBA] Generating flipMove table...\n";
    
    CubieCube cube = CubieCube();
    auto orients = rcube::Orientation::iterate();

    for (int i = 0; i < N_FLIP; ++i)
    {
        cube.setFlip(i);

        for (int k = 0; k < 6; ++k)
        {
            rcube::Move mv(orients[k], 1);

            for (int j = 0; j < 3; ++j)
            {
                cube.edgeMultiply(mv);
                flipMove[i][3 * k + j] = cube.getFlip();
            }
            cube.edgeMultiply(mv); // reset the cube's state
        }
    }
    writeFile((char*)flipMove, sizeof(flipMove), "flipMove", path);

    std::cout << "[KOCIEMBA] Generated flipMove table\n";
}

void generateSliceSortedMove(const std::string &path)
{
    std::cout << "[KOCIEMBA] Generating sliceSortedMove table...\n";
    
    CubieCube cube = CubieCube();
    auto orients = rcube::Orientation::iterate();

    for (int i = 0; i < N_SLICE_SORTED; ++i)
    {
        cube.setSliceSorted(i);

        for (int k = 0; k < 6; ++k)
        {
            rcube::Move mv(orients[k], 1);

            for (int j = 0; j < 3; ++j)
            {
                cube.edgeMultiply(mv);
                sliceSortedMove[i][3 * k + j] = cube.getSliceSorted();
            }
            cube.edgeMultiply(mv); // reset the cube's state
        }
    }
    writeFile((char*)sliceSortedMove, sizeof(sliceSortedMove),
        "sliceSortedMove", path);

    std::cout << "[KOCIEMBA] Generated sliceSortedMove table\n";
}

void generateSliceTwistPrun(const std::string &path)
{
    std::cout << "[KOCIEMBA] Generating sliceTwistPrun table...\n";

    // This table is filled recursively: first 0 is assigned to index 0
    // (where twist=0 and slice=0), then all the 18 moves are applied and 1 is
    // assigned to all the combinations of twist and slice values found.
    // In general, at the i-th iteration, all the combinations of twist and slice
    // for which the table value is i-1 are taken, all the 18 moves are applied
    // to them and i is written in the table at the index corresponding to the
    // combinations of twist and slice found (if a smaller value was not written
    // already). This makes sure that all the values are minimal. The cycle
    // stops when all the table has been filled.

    int depth = 0, done = 1;

    for (int i = 0; i < N_SLICE * N_TWIST / 2 + 1; ++i)
        sliceTwistPrun[i] = -1; // 11111111 in binary

    writeTable(sliceTwistPrun, 0, 0);
    while (done != N_SLICE * N_TWIST)
    {
        for (int i = 0; i < N_SLICE * N_TWIST; ++i)
        {
            if (readTable(sliceTwistPrun, i) != depth) continue;

            int twist = i / N_SLICE;
            int slice = i % N_SLICE;

            for (int j = 0; j < 18; ++j)
            {
                int newSlice = sliceSortedMove[slice * 24][j] / 24;
                int newTwist = twistMove[twist][j];

                if (readTable(sliceTwistPrun, N_SLICE * newTwist + newSlice)
                    != 0x0f)
                    continue; // The table has already been written at this index

                writeTable(sliceTwistPrun, N_SLICE * newTwist + newSlice,
                    (signed char)(depth + 1));
                done++;
            }
        }
        depth++;
    }
    writeFile((char*)sliceTwistPrun, sizeof(sliceTwistPrun),
        "sliceTwistPrun", path);

    std::cout << "[KOCIEMBA] Generated sliceTwistPrun table\n";
}

void generateSliceFlipPrun(const std::string &path)
{
    std::cout << "[KOCIEMBA] Generating sliceFlipPrun table...\n";

    // This table is filled with the same algorithm as sliceTwistPrun

    int depth = 0, done = 1;

    for (int i = 0; i < N_SLICE * N_FLIP / 2; ++i)
        sliceFlipPrun[i] = -1; // 11111111 in binary

    writeTable(sliceFlipPrun, 0, 0);
    while (done != N_SLICE * N_FLIP)
    {
        for (int i = 0; i < N_SLICE * N_FLIP; ++i)
        {
            if (readTable(sliceFlipPrun, i) != depth) continue;

            int flip = i / N_SLICE;
            int slice = i % N_SLICE;

            for (int j = 0; j < 18; ++j)
            {
                int newSlice = sliceSortedMove[slice * 24][j] / 24;
                int newFlip = flipMove[flip][j];
                
                if (readTable(sliceFlipPrun, N_SLICE * newFlip + newSlice)
                    != 0x0f) continue;

                writeTable(sliceFlipPrun, N_SLICE * newFlip + newSlice,
                    (signed char) (depth + 1));
                done++;
            }
        }
        depth++;
    }
    writeFile((char*)sliceFlipPrun, sizeof(sliceFlipPrun),
        "sliceFlipPrun", path);

    std::cout << "[KOCIEMBA] Generated sliceFlipPrun table\n";
}

void generateTable(const std::string &table, const std::string path)
{
    if (table == "flipMove")        {generateFlipMove(path); return;}
    if (table == "twistMove")       {generateTwistMove(path); return;}
    if (table == "sliceSortedMove") {generateSliceSortedMove(path); return;}

    // sliceTwistPrun and sliceFlipPrun require the first 3 tables to be loaded
    readFile((char*)twistMove, sizeof(twistMove), path + "/" + "twistMove");
    readFile((char*)flipMove, sizeof(flipMove), path + "/" + "flipMove");
    readFile((char*)sliceSortedMove, sizeof(sliceSortedMove), path + "/" +
        "sliceSortedMove");

    if      (table == "sliceTwistPrun")  generateSliceTwistPrun(path);
    else if (table == "sliceFlipPrun")   generateSliceFlipPrun(path);
}

void initTables()
{
    std::string path = STD_PATH;
    std::string tables[5] = {"twistMove", "flipMove", "sliceSortedMove",
        "sliceTwistPrun", "sliceFlipPrun"};
    
    for (int i = 0; i < 5; ++i)
    {
        while (true)
        {
            if (!fileExists(path + "/" + tables[i]))
            {
                std::cout << "[KOCIEMBA] Cannot find table " << tables[i] <<
                    " at path '" << path << "'\n";
                std::cout << "[KOCIEMBA] What would you like to do?"
                " [P (change path)/G (regenerate the table)] ";

                char ans;
                std::cin >> ans;

                switch (ans)
                {
                    case 'G': generateTable(tables[i], path); break;
                    case 'P':
                    {
                        std::cout << "[KOCIEMBA] Ener a new path: ";
                        std::cin >> path;
                    }
                    default: continue;
                }
            }
            else break;
        }
    }

    std::cout << "[KOCIEMBA] Loading lookup tables...\n";

    readFile((char*)twistMove, sizeof(twistMove), path + "/" + "twistMove");
    readFile((char*)flipMove, sizeof(flipMove), path + "/" + "flipMove");
    readFile((char*)sliceSortedMove, sizeof(sliceSortedMove),
        path + "/" + "sliceSortedMove");
    readFile((char*)sliceTwistPrun, sizeof(sliceTwistPrun),
        path + "/" + "sliceTwistPrun");
    readFile((char*)sliceFlipPrun, sizeof(sliceFlipPrun),
        path + "/" + "sliceFlipPrun");
}

} // namespace Kociemba