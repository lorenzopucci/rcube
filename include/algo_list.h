/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <map>

std::map<std::string, std::string> algorithms {
    {"sexy-move", "RUR'U'"},
    {"inv-sexy-move", "RUR'U'"},
    {"sune", "RUR'UR2UR'"},
    {"anti-sune", "L'U'LU'L'2U'L"},
    {"Ub-perm", "R2U(RUR'U')R'U'(R'UR')"},
    {"Ua-perm", "(RU'RU)RU(RU'R'U')R2"},
    {"Z-perm", "M2UM2UM'U2M2U2M'U2"},
    {"H-perm", "M2UM2U2M2UM2"},
    {"Aa-perm", "x(R'UR')D2(RU'R')D2R2x'"},
    {"Ab-perm", "xR2'D2(RUR')D2(RU'R)x'"},
    {"E-perm", "x'(RU'R'D)(RUR'D')(RUR'D)(RU'R'D')x"},
    {"Ra-perm", "(RU'R'U')(RURD)(R'U'RD')(R'U2R')U'"},
    {"Rb-perm", "(R'U2RU2')R'F(RUR'U')R'F'R2U'"},
    {"Ja-perm", "(R'UL'U2)(RU'R'U2RLU'"},
    {"Jb-perm", "(RUR'F')(RUR'U')R'FR2U'R'U'"},
    {"T-perm", "(RUR'U')(R'FR2U')R'U'(RUR'F')"},
    {"F-perm", "(R'U'F')(RUR'U')(R'FR2U')(R'U'RU)(R'UR)"}
}