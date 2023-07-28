/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <map>

#include "cubieCube.hpp"
#include "misc.hpp"

namespace Kociemba
{

// cPerm, cOri, ePerm, eOri values for all the 6 basic move cubes
static Corner cpU[8] = {UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB};
static uint8_t coU[8]  = {0, 0, 0, 0, 0, 0, 0, 0};
static Edge epU[12] = {UB, UR, UF, UL, DR, DF, DL, DB, FR, FL, BL, BR};
static uint8_t eoU[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Corner cpR[8] = {DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR};
static uint8_t coR[8]  = {2, 0, 0, 1, 1, 0, 0, 2};
static Edge epR[12] = {FR, UF, UL, UB, BR, DF, DL, DB, DR, FL, BL, UR};
static uint8_t eoR[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Corner cpF[8] = {UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB};
static uint8_t coF[8]  = {1, 2, 0, 0, 2, 1, 0, 0};
static Edge epF[12] = {UR, FL, UL, UB, DR, FR, DL, DB, UF, DF, BL, BR};
static uint8_t eoF[12] = {0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0};
static Corner cpD[8] = {URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR};
static uint8_t coD[8]  = {0, 0, 0, 0, 0, 0, 0, 0};
static Edge epD[12] = {UR, UF, UL, UB, DF, DL, DB, DR, FR, FL, BL, BR};
static uint8_t eoD[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Corner cpL[8] = {URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB};
static uint8_t coL[8]  = {0, 1, 2, 0, 0, 2, 1, 0};
static Edge epL[12] = {UR, UF, BL, UB, DR, DF, FL, DB, FR, UL, DL, BR};
static uint8_t eoL[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static Corner cpB[8] = {URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL};
static uint8_t coB[8]  = {0, 0, 1, 2, 0, 0, 2, 1};
static Edge epB[12] = {UR, UF, UL, BR, DR, DF, DL, BL, FR, FL, UB, DB};
static uint8_t eoB[12] = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1};


// needed to convert a rcube::Cube to a Kociemba::CubieCube
std::map<std::vector<rcube::Orientation>,Corner> orientsToCorner = {
    {{{Axis::Y, 1}, {Axis::X, 1}, {Axis::Z, 1}}, URF},
    {{{Axis::Y, 1}, {Axis::Z, 1}, {Axis::X, -1}}, UFL},
    {{{Axis::Y, 1}, {Axis::X, -1}, {Axis::Z, -1}}, ULB},
    {{{Axis::Y, 1}, {Axis::Z, -1}, {Axis::X, 1}}, UBR},
    {{{Axis::Y, -1}, {Axis::Z, 1}, {Axis::X, 1}}, DFR},
    {{{Axis::Y, -1}, {Axis::X, -1}, {Axis::Z, 1}}, DLF},
    {{{Axis::Y, -1}, {Axis::Z, -1}, {Axis::X, -1}}, DBL},
    {{{Axis::Y, -1}, {Axis::X, 1}, {Axis::Z, -1}}, DRB}
};

// needed to convert a rcube::Cube to a Kociemba::CubieCube
std::map<std::vector<rcube::Orientation>,Edge> orientsToEdge = {
    {{{Axis::Y, 1}, {Axis::X, 1}}, UR},
    {{{Axis::Y, 1}, {Axis::Z, 1}}, UF},
    {{{Axis::Y, 1}, {Axis::X, -1}}, UL},
    {{{Axis::Y, 1}, {Axis::Z, -1}}, UB},
    {{{Axis::Y, -1}, {Axis::X, 1}}, DR},
    {{{Axis::Y, -1}, {Axis::Z, 1}}, DF},
    {{{Axis::Y, -1}, {Axis::X, -1}}, DL},
    {{{Axis::Y, -1}, {Axis::Z, -1}}, DB},
    {{{Axis::Z, 1}, {Axis::X, 1}}, FR},
    {{{Axis::Z, 1}, {Axis::X, -1}}, FL},
    {{{Axis::Z, -1}, {Axis::X, -1}}, BL},
    {{{Axis::Z, -1}, {Axis::X, 1}}, BR},
};


CubieCube::CubieCube()
{
    restoreCube();
}

CubieCube::CubieCube(rcube::Cube cube)
{
    restoreCube();

    // manage corners
    for (auto it = orientsToCorner.begin(); it != orientsToCorner.end(); ++it)
    {
        rcube::Coordinates cPos(it->first[0], it->first[1], it->first[2]);

        // get the colors of the corner at cPos
        Color cols[3] = {
            cube.getStickerAt(cPos, it->first[0]),
            cube.getStickerAt(cPos, it->first[1]),
            cube.getStickerAt(cPos, it->first[2])
        };

        // convert all colors to the orientation of the corresponging face
        // (the one which as the center of that color).
        rcube::Orientation orients[3] = {
            cube.getFaceOrientation(cols[0]),
            cube.getFaceOrientation(cols[1]),
            cube.getFaceOrientation(cols[2]),
        };

        // get the coordinates that the corner would have if the cube were solved
        rcube::Coordinates cubie(orients[0], orients[1], orients[2]);

        // find the corresponding index in orientsToCorner
        auto it1 = orientsToCorner.begin();
        for (; it1 != orientsToCorner.end(); ++it1)
        {
            if (rcube::Coordinates(it1->first[0], it1->first[1], it1->first[2])
                == cubie) break;
        }

        // which color between white and yellow is in the corner
        Color udColor = cube.getFaceColor({Axis::Y, cubie.y()});

        int ori = 0;
        for (; ori < 3; ++ori)
        {
            if (cube.getStickerAt(cPos, it->first[ori]) == udColor) break;
        }

        cPerm[(int)it->second] = it1->second;
        cOri[(int)it->second] = ori;
    }

    // manage edges
    for (auto it = orientsToEdge.begin(); it != orientsToEdge.end(); ++it)
    {
        rcube::Coordinates ePos(it->first[0], it->first[1]);

        // get the colors of the edge at ePos
        Color cols[2] = {
            cube.getStickerAt(ePos, it->first[0]),
            cube.getStickerAt(ePos, it->first[1])
        };

        // convert all colors to the orientation of the corresponging face
        // (the one which as the center of that color).
        rcube::Orientation orients[2] = {
            cube.getFaceOrientation(cols[0]),
            cube.getFaceOrientation(cols[1])
        };

        // get the coordinates that the corner would have if the cube were solved
        rcube::Coordinates cubie(orients[0], orients[1]);

        // find the corresponding index in orientsToCorner
        auto it1 = orientsToEdge.begin();
        for (; it1 != orientsToEdge.end(); ++it1)
        {
            if (rcube::Coordinates(it1->first[0], it1->first[1]) == cubie)
                break;
        }

        // compute the edge's orientation
        int ori = 0;
        if (cube.getFaceColor(it1->first[0]) != cols[0]) ori = 1;

        ePerm[(int)it->second] = it1->second;
        eOri[(int)it->second] = ori;
    }
}

CubieCube::CubieCube(const rcube::Move &mv)
{
    // When direction != 1 the basic move cubes are multiplied by themselves
    // for direction times.
    if (mv.direction % 4 != 1)
    {
        restoreCube();
        CubieCube moveCube(rcube::Move(mv.face, 1));

        for (int i = 0; i < (mv.direction + 4) % 4; ++i) multiply(moveCube);
        return;
    }

    // When direction = 1, the move cube is initialiezed with the values
    // declared at the beginning of this file.

    Edge *ep;
    Corner *cp;
    uint8_t *eo, *co;

    switch (mv.face)
    {
        case UP:    ep = epU; cp = cpU; eo = eoU; co = coU; break;
        case DOWN:  ep = epD; cp = cpD; eo = eoD; co = coD; break;
        case LEFT:  ep = epL; cp = cpL; eo = eoL; co = coL; break;
        case RIGHT: ep = epR; cp = cpR; eo = eoR; co = coR; break;
        case FRONT: ep = epF; cp = cpF; eo = eoF; co = coF; break;
        case BACK:  ep = epB; cp = cpB; eo = eoB; co = coB; break;
    }

    for (int i = 0; i < 12; ++i)
    {
        ePerm[i] = ep[i];
        eOri[i] = eo[i];

        if (i > 7) continue;

        cPerm[i] = cp[i];
        cOri[i] = co[i];
    }
}

void CubieCube::edgeMultiply(const CubieCube &c)
{
    Edge newEPerm[12];
    uint8_t newEOri[12];

    for (int i = 0; i < 12; ++i)
    {
        newEPerm[i] = ePerm[c.ePerm[i]];
        newEOri[i] = (eOri[c.ePerm[i]] + c.eOri[i]) % 2;
    }

    for (int i = 0; i < 12; ++i)
    {
        ePerm[i] = newEPerm[i];
        eOri[i] = newEOri[i];
    }
}

void CubieCube::cornerMultiply(const CubieCube &c)
{
    Corner newCPerm[8];
    uint8_t newCOri[8];

    for (int i = 0; i < 8; ++i)
    {
        newCPerm[i] = cPerm[c.cPerm[i]];
        newCOri[i] = (cOri[c.cPerm[i]] + c.cOri[i]) % 3;
    }

    for (int i = 0; i < 8; ++i)
    {
        cPerm[i] = newCPerm[i];
        cOri[i] = newCOri[i];
    }
}

void CubieCube::multiply(const CubieCube &c)
{
    cornerMultiply(c);
    edgeMultiply(c);
}

void CubieCube::performMove(const rcube::Move &mv)
{
    multiply(CubieCube(mv));
}

void CubieCube::restoreCube()
{
    for (int i = 0; i < 12; ++i)
    {
        ePerm[i] = (Edge)(i);
        eOri[i] = 0;

        if (i > 7) continue;

        cPerm[i] = (Corner)(i);
        cOri[i] = 0;
    }
}

uint16_t CubieCube::getFlip()
{
    uint16_t res = 0;
    for (int i = 0; i < 11; ++i)
    {
        res *= 2;
        res += eOri[i];
    }
    return res;
}

void CubieCube::setFlip(uint16_t flip)
{
    int flipParity = 0;

    for (int i = 10; i >= 0; --i)
    {
        eOri[i] = flip % 2;
        flipParity += eOri[i];
        flip /= 2;
    }
    eOri[11] = (2 - (flipParity % 2)) % 2;
}

uint16_t CubieCube::getTwist()
{
    uint16_t res = 0;

    for (int i = 0; i < 7; ++i)
    {
        res *= 3;
        res += cOri[i];
    }
    return res;
}

void CubieCube::setTwist(uint16_t twist)
{
    int twistParity = 0;

    for (int i = 6; i >= 0; --i)
    {
        cOri[i] = twist % 3;
        twistParity += cOri[i];
        twist /= 3;
    }
    cOri[7] = (3 - (twistParity % 3)) % 3;
}

template<typename T>
void rotateRight(T* array, int left, int right)
{
    // shift to the right of one position the elements of an array between
    // indices left and right

    T tmp = array[right];
    for (int i = right; i > left; --i)
        array[i] = array[i - 1];
    array[left] = tmp;
}

template<typename T>
void rotateLeft(T* array, int left, int right)
{
    // shift to the left of one position the elements of an array between
    // indices left and right

    T tmp = array[left];
    for (int i = left; i < right; ++i)
        array[i] = array[i + 1];
    array[right] = tmp;
}

uint16_t CubieCube::getSliceSorted()
{
    uint16_t a = 0, b = 0, x = 0;
    Edge edges[4];

    for (int i = 11; i >= 0; --i)
    {
        if (ePerm[i] >= Edge::FR && ePerm[i] <= Edge::BR)
        {
            a += choose(11 - i, x + 1);
            edges[3 - x] = ePerm[i];
            x++;
        }
    }
    for (int i = 3; i > 0; --i)
    {
        int k = 0;

        while (edges[i] != i + 8)
        {
            rotateLeft(edges, 0, i);
            k++;
        }
        b = (i + 1) * b + k;
    }
    return 24 * a + b;
}

void CubieCube::setSliceSorted(uint16_t yEdges)
{
    Edge sliceEdges[4] = {FR, FL, BL, BR};
    Edge otherEdges[8] = {UR, UF, UL, UB, DR, DF, DL, DB};

    int a = yEdges / 24;
    int b = yEdges % 24;

    for (int i = 0; i < 12; ++i) ePerm[i] = Edge::E_NONE;

    for (int i = 1; i < 4; ++i)
    {
        int k = b % (i + 1);
        b /= i + 1;
        while (k > 0)
        {
            rotateRight(sliceEdges, 0, i);
            k--;
        }
    }
    int x = 4;
    for (int i = 0; i < 12; ++i)
    {
        if (a - choose(11 - i, x) >= 0)
        {
            ePerm[i] = sliceEdges[4 - x];
            a -= choose(11 - i, x);
            x--;
        }
    }
    x = 0;
    for (int i = 0; i < 12; ++i)
    {
        if (ePerm[i] == Edge::E_NONE)
        {
            ePerm[i] = otherEdges[x];
            x++;
        }
    }
}

uint16_t CubieCube::getUDEdges()
{
    Edge edges[8];
    int res = 0;

    for (int i = 0; i < 8; ++i)
    {
        if (ePerm[i] >= Edge::FR) return 50000;
        edges[i] = ePerm[i];
    }
    for (int i = 7; i > 0; --i)
    {
        int k = 0;
        while (edges[i] != i)
        {
            rotateLeft(edges, 0, i);
            k++;
        }
        res *= i + 1;
        res += k;
    }
    return res;
}

void CubieCube::setUDEdges(uint16_t udEdges)
{
    Edge edges[8] = {UR, UF, UL, UB, DR, DF, DL, DB};
    Edge otherEdges[4] = {FR, FL, BL, BR};

    for (int i = 0; i < 12; ++i) ePerm[i] = Edge::E_NONE;

    for (int i = 1; i < 8; ++i)
    {
        int k = udEdges % (i + 1);
        udEdges /= i + 1;
        while (k > 0)
        {
            rotateRight(edges, 0, i);
            --k;
        }
    }

    for (int i = 0; i < 8; ++i) ePerm[i] = edges[i];
    for (int i = 0; i < 4; ++i) ePerm[i + 8] = otherEdges[i];
}

uint16_t CubieCube::getCorners()
{
    Corner corners[8] = {(Corner)0};
    int res = 0;

    for (int i = 0; i < 8; ++i) corners[i] = cPerm[i];

    for (int i = 7; i > 0; --i)
    {
        int k = 0;
        while (corners[i] != i)
        {
            rotateLeft(corners, 0, i);
            k++;
        }
        res *= i + 1;
        res += k;
    }
    return res;
}

void CubieCube::setCorners(uint16_t value)
{
    Corner corners[8] = {URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB};

    for (int i = 1; i < 8; ++i)
    {
        int k = value % (i + 1);
        value /= i + 1;
        while (k > 0)
        {
            rotateRight(corners, 0, i);
            --k;
        }
    }

    for (int i = 0; i < 8; ++i) cPerm[i] = corners[i];
}

} // namespace Kociemba