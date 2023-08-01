/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <cstdint>
#include <rcube.hpp>

namespace Kociemba
{
    enum Edge
    {
        UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR, E_NONE
    };

    enum Corner
    {
        URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB, C_NONE
    };

    class CubieCube
    {
    /*
    * This class is used to store the state of the cube and the 18 possible
    * moves. For the purpose of Kociemba's algorithm, this is a better way
    * to store the state of the cube than rcube::Cube.
    */

    public:
        // initialize a solved cube
        CubieCube();

        // initialize by copying the state of a rcube::Cube
        CubieCube(rcube::Cube cube);

        // initialize a move cube: this is basically a solved cube with only
        // the desired move applied
        CubieCube(const rcube::Move &move);

        // A multiplication is an operation between two CubieCubes A and B
        // defined so as to permute and orient all the corners and edges of A
        // as it would be required to take a solved cube to state B. This is
        // useful when B is a move cube: multipying A times B means applying move
        // B to A

        // multiplication limited to the corners
        void cornerMultiply(const CubieCube &c);

        // multiplication limited to the edges
        void edgeMultiply(const CubieCube &c);

        // full multiplication
        void multiply(const CubieCube &c);

        // apply a rcube::Move (this boils down to a multiplication)
        void performMove(const rcube::Move &mv);

        // apply a rcube::Algorithm
        void performAlgorithm(const rcube::Algorithm &algo);

        // restore the cube to the solved state
        void restoreCube();

        // The flip is a number between 0 and 2048 that contains all the
        // orientations of the 12 edges (only 11 actually, the last one is
        // deduced from the others). Its digits in base 2 are the single
        // orientations of the edges.

        // Encode orientations into a flip value
        uint16_t getFlip();

        // Decode a flip value and apply it the the cube
        void setFlip(uint16_t twist);

        // The twist is a number between 0 and 2187 that contains all the
        // orientations of the 8 corners (only 7 actually, like for flip). Its
        // digits in base 3 are the single orientations of the corners.

        // Encode orientations into a twist value
        uint16_t getTwist();

        // Decode a twist value and apply it to the cube
        void setTwist(uint16_t twist);

        // SliceSorted is a value that represents the permutation of the 4
        // edges of the central slice (FR, FL, BL, BR)
        uint16_t getSliceSorted();
        void setSliceSorted(uint16_t sliceSorted);

        // udEdges is a value that represents the permutation of the 8 edges
        // of the U and D faces, assuming the cube is in G1 (these edges are only
        // in the U and D faces)
        uint16_t getUDEdges();
        void setUDEdges(uint16_t udEdges);

        // Corners is a value that represents the permutation of the 8 corners.
        uint16_t getCorners();
        void setCorners(uint16_t corners);

        uint8_t getCornerParity();
        uint8_t getEdgeParity();

        Edge ePerm[12];
        uint8_t eOri[12];

        Corner cPerm[8];
        uint8_t cOri[8];
    };

};