/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <vector>
#include <string>

#include "algoDButil.hpp"

std::vector<AlgoDBItem> algoDb
{
    // general
    {"sexy-move", "RUR'U'", GENERAL},
    {"inv-sexy-move", "RUR'U'", GENERAL},
    {"sune", "RUR'URU2R'", OLL, "aAaAAAAAa"}, // ambiguous
    {"anti-sune", "L'U'LU'L'U2L", OLL, "aAaAAAAAa"}, // ambiguous


    // OLL
    {"OCLL1", "(RU2R')(U'RUR')(U'RU'R')", OLL, "AaAaaaAaAaaa"},
    {"OCLL2", "RU2R2U'R2U'R2U2R", OLL, "aaAaaaAaaAaA"},
    {"OCLL3", "R2D(R'U2R)D'(R'U2R')", OLL, "aaaaaaAaAaaa"},
    {"OCLL4", "(rUR'U')(r'FRF')", OLL, "AaaaaaaaAaaa"},
    {"OCLL5", "yF'(rUR'U')r'FR", OLL, "aaaAaaaaAaaa"},
    {"OCLL6", "RU2R'U'RU'R'", OLL, "aAaAAAAAa"}, // ambiguous
    {"OCLL7", "RUR'URU2R'", OLL, "aAaAAAAAa"}, // ambiguous

    {"T1", "(RUR'U')(R'FRF')", OLL},
    {"T2", "F(RUR'U')F'", OLL},

    {"S1", "(r'U2RUR'Ur)", OLL},
    {"S2", "(rU2R'U'RU'r')", OLL},

    {"C1", "(RUR2U')(R'FRU)RU'F'", OLL},
    {"C2", "R'U'(R'FRF')UR", OLL},

    {"W1", "(R'U'RU')(R'URU)lU'R'Ux", OLL},
    {"W2", "(RUR'U)(RU'R'U')(R'FRF')", OLL},

    {"E1", "(rUR'U')M(URU'R')", OLL},
    {"E2", "(RUR'U')M'(URU'r')", OLL},

    {"P1", "(R'U'F)(URU'R')F'R", OLL},
    {"P2", "RUB'(U'R'U)(RBR')", OLL},
    {"P3", "yR'U'F'UFR", OLL},
    {"P4", "f(RUR'U')f'", OLL},

    {"I1", "f(RUR'U')(RUR'U')f'", OLL},
    {"I2", "(R'U'RU'R'U)y'(R'UR)B", OLL},
    {"I3", "y(R'FRU)(RU'R2F')R2U'R'(URUR')", OLL},
    {"I4", "r'U'r(U'R'UR)(U'R'UR)r'Ur", OLL},

    {"F1", "(RUR'U')R'F(R2UR'U')F'", OLL},
    {"F2", "(RUR'U)(R'FRF')(RU2R')", OLL},
    {"F3", "(RU2)(R2FRF')(RU2R')", OLL},
    {"F4", "F(RU'R'U')(RUR'F')", OLL},

    {"K1", "(rU'r')(U'rUr')y'(R'UR)", OLL},
    {"K2", "(R'FR)(UR'F'R)(FU'F')", OLL},
    {"K3", "(r'U'r)(R'U'RU)(r'Ur)", OLL},
    {"K4", "(rUr')(RUR'U')(rU'r')", OLL},

    {"A1", "y(RUR'U')(RU'R')(F'U'F)(RUR')", OLL},
    {"A2", "y'FU(RU2R'U')(RU2R'U')F'", OLL},
    {"A3", "(RUR'URU2R')F(RUR'U')F'", OLL},
    {"A4", "(R'U'RU'R'U2R)F(RUR'U')F'", OLL},

    {"L1", "F'(L'U'LU)(L'U'LU)F", OLL},
    {"L2", "F(RUR'U')(RUR'U')F'", OLL},
    {"L3", "rU'r2Ur2Ur2U'r", OLL},
    {"L4", "r'Ur2U'r2U'r2Ur'", OLL},
    {"L5", "(r'U'RU')(R'URU')R'U2r", OLL},
    {"L6", "(rUR'U)(RU'R'U)RU2r'", OLL},

    {"B1", "(rUR'URU2r')", OLL},
    {"B2", "(r'U'RU'R'U2r)", OLL},
    {"B3", "r'(R2UR'URU2R')UM'", OLL},
    {"B4", "M'(R'U'RU'R'U2R)U'M", OLL},
    {"B5", "(LF')(L'U'LU)FU'L'", OLL},
    {"B6", "(R'F)(RUR'U')F'UR", OLL},

    {"O1", "(RU2)(R2FRF')U2(R'FRF')", OLL},
    {"O2", "F(RUR'U')F'f(RUR'U')f'", OLL},
    {"O3", "f(RUR'U')f'U'F(RUR'U')F'", OLL},
    {"O4", "f(RUR'U')f'UF(RUR'U')F'", OLL},
    {"O5", "(RUR'U)(R'FRF')U2(R'FRF')", OLL},
    {"O6", "yRU2(R2FRF')U2M'(URU'r')", OLL},
    {"O7", "MU(RUR'U')M'(R'FRF')", OLL},
    {"O8", "MU(RUR'U')M2(URU'r')", OLL},



    // PLL
    {"Ub-perm", "R2U(RUR'U')R'U'(R'UR')", PLL, "MMMNmNmnmnNn"}, // ambiguous
    {"Ua-perm", "(RU'RU)RU(RU'R'U')R2", PLL, "MMMNmNmnmnNn"}, // ambiguous
    {"Z-perm", "M2UM2UM'U2M2U2M'U2", PLL, "MNMnmnmnmNMN"},
    {"H-perm", "M2UM2U2M2UM2", PLL, "MmMNnNmMmnNn"},

    {"Aa-perm", "x(R'UR')D2(RU'R')D2R2x'", PLL},
    {"Ab-perm", "xR2D2(RUR')D2(RU'R)x'", PLL},
    {"E-perm", "x'(RU'R'D)(RUR'D')(RUR'D)(RU'R'D')x", PLL},

    {"Ra-perm", "(RU'R'U')(RURD)(R'U'RD')(R'U2R')U'", PLL},
    {"Rb-perm", "(R'U2RU2)R'F(RUR'U')R'F'R2U'", PLL},
    {"Ja-perm", "(R'UL'U2)(RU'R'U2RLU'", PLL},
    {"Jb-perm", "(RUR'F')(RUR'U')R'FR2U'R'U'", PLL},
    {"T-perm", "(RUR'U')(R'FR2U')R'U'(RUR'F')", PLL, "M*Nm*MN*mn*n"},
    {"F-perm", "(R'U'F')(RUR'U')(R'FR2U')(R'U'RU)(R'UR)", PLL},

    {"V-perm", "(R'UR'U')y(R'F'R2U')(R'UR'F)RF", PLL},
    {"Y-perm", "F(RU'R'U')(RUR'F')(RUR'U')(R'FRF')", PLL, "M*mN*nm*Mn*N"},
    {"Na-perm", "(RUR'U)(RUR'F')(RUR'U')(R'FR2U')R'U2(RU'R')", PLL},
    {"Nb-perm", "(R'URU')(R'F'U'F)(RUR'F)R'F'(RU'R)", PLL},
    
    {"Ga-perm", "R2U(R'UR'U')(RU'R2)DU'(R'URD')U", PLL},
    {"Gb-perm", "y'D(R'U'RU)D'(R2UR'U)(RU'RU')R2U'", PLL},
    {"Gc-perm", "R2U'(RU'RU)(R'UR2D')(URU'R')DU'", PLL},
    {"Gd-perm", "D'(RUR'U')D(R2U'RU')(R'UR'U)R2U", PLL}
};