/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include "misc.hpp"

namespace Kociemba
{

int getFaceNumber(const rcube::Orientation &orient)
{
    return (int)orient.axis * 2 + ((orient.direction + 1) / 2);
}

int toPh2Move(int ph1Move)
{
    if (ph1Move >= 6 && ph1Move < 12) return ph1Move - 6;
    switch (ph1Move)
    {
        case 1: return 6;
        case 4: return 7;
        case 13: return 8;
        case 16: return 9;
        default: return -1;
    }
}

int toPh1Move(int ph2Move)
{
    if (ph2Move < 6) return ph2Move + 6;
    switch (ph2Move)
    {
        case 6: return 1;
        case 7: return 4;
        case 8: return 13;
        case 9: return 16;
        default: return -1;
    }
}

int choose(int n, int k)
{
    if (n < k) return 0;
    if (k > n/2) k = n - k;

    int res = 1;

    for (int i = n, j = 1; i != n - k; i--, j++)
    {
        res *= i;
        res /= j;
    }
    return res;
}

}; // namespace Kociemba