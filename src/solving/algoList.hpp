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

/*
* Algorithm database
* Most of the algorithm here are from cubeskills.com. They are divided into
* multiple types:
* - GENERAL
* - OLL: they all contain a regex for rcube::Cube::layerAndFaceMatch to
*   identify them
* - PLL: they all contain a regex for rcube::Cube::layerMatches to identify
*   them
*/

std::vector<AlgoDBItem> algoDb
{
    // general
    {"sexy-move", "RUR'U'", GENERAL},
    {"inv-sexy-move", "RUR'U'", GENERAL},
    {"sune", "RUR'URU2R'", OLL, "aAaAAAAAa-AaaAaaAaaaaa"},
    {"anti-sune", "L'U'LU'L'U2L", OLL, "aAaAAAAAa-aaAaaaaaAaaA"},


    // OLL
    {"OCLL1", "(RU2R')(U'RUR')(U'RU'R')", OLL, "aAaAAAaAa-AaAaaaAaAaaa"},
    {"OCLL2", "RU2R2U'R2U'R2U2R", OLL, "aAaAAAaAa-aaAaaaAaaAaA"},
    {"OCLL3", "R2D(R'U2R)D'(R'U2R')", OLL, "AAAAAAaAa-aaaaaaAaAaaa"},
    {"OCLL4", "(rUR'U')(r'FRF')", OLL, "aAAAAAaAA-AaaaaaaaAaaa"},
    {"OCLL5", "yF'(rUR'U')r'FR", OLL, "AAaAAAaAA-aaaAaaaaAaaa"},
    {"OCLL6", "RU2R'U'RU'R'", OLL, "aAAAAAaAa-aaaaaAaaAaaA"},
    {"OCLL7", "RUR'URU2R'", OLL, "aAaAAAAAa-AaaAaaAaaaaa"},

    {"T1", "(RUR'U')(R'FRF')", OLL, "aaAAAAaaA-AAaaaaaAAaaa"},
    {"T2", "F(RUR'U')F'", OLL, "aaAAAAaaA-aAaaaaaAaAaA"},

    {"S1", "(r'U2RUR'Ur)", OLL, "aaaaAAaAA-AAaAaaaaaAAa"},
    {"S2", "(rU2R'U'RU'r')", OLL, "aAAaAAaaa-aaaaaAaAAaAA"},

    {"C1", "(RUR2U')(R'FRU)RU'F'", OLL, "aaaAAAAaA-aAaAaaaAaaaA"},
    {"C2", "R'U'(R'FRF')UR", OLL, "AAaaAaAAa-aaaAAAaaaaAa"},

    {"W1", "(R'U'RU')(R'URU)lU'R'Ux", OLL, "AaaAAaaAA-aAaAAaaaAaaa"},
    {"W2", "(RUR'U)(RU'R'U')(R'FRF')", OLL, "aAAAAaAaa-AaaaAAaAaaaa"},

    {"E1", "(rUR'U')M(URU'R')", OLL, "AAAAAaAaA-aaaaAaaAaaaa"},
    {"E2", "(RUR'U')M'(URU'r')", OLL, "AaAAAAAaA-aAaaaaaAaaaa"},

    {"P1", "(R'U'F)(URU'R')F'R", OLL, "aAAaAAaaA-AaaaaaaAAaAa"},
    {"P2", "RUB'(U'R'U)(RBR')", OLL, "aaAaAAaAA-AAaaaaaaAaAa"},
    {"P3", "yR'U'F'UFR", OLL, "AaaAAaAAa-aAaAAAaaaaaa"},
    {"P4", "f(RUR'U')f'", OLL, "aaAaAAaAA-aAaaaaaaaAAA"},

    {"I1", "f(RUR'U')(RUR'U')f'", OLL, "aaaAAAaaa-aAAaaaAAaAaA"},
    {"I2", "(R'U'RU'R'U)y'(R'UR)B", OLL, "aAaaAaaAa-AaaAAAaaAaAa"},
    {"I3", "y(R'FRU)(RU'R2F')R2U'R'(URUR')", OLL, "aAaaAaaAa-aaaAAAaaaAAA"},
    {"I4", "r'U'r(U'R'UR)(U'R'UR)r'Ur", OLL, "aaaAAAaaa-aAaAaAaAaAaA"},

    {"F1", "(RUR'U')R'F(R2UR'U')F'", OLL, "aAaAAaaaA-aaAaAaaAAaaA"},
    {"F2", "(RUR'U)(R'FRF')(RU2R')", OLL, "aaAAAaaAa-AAaaAaAaaAaa"},
    {"F3", "(RU2)(R2FRF')(RU2R')", OLL, "AaaaAAaAA-aAaAaaaaAaAa"},
    {"F4", "F(RU'R'U')(RUR'F')", OLL, "AAaAAaaaA-aaaAAaaAAaaa"},

    {"K1", "(rU'r')(U'rUr')y'(R'UR)", OLL, "aaaAAAAaa-AAaAaaAAaaaa"},
    {"K2", "(R'FR)(UR'F'R)(FU'F')", OLL, "aaaAAAaaA-aAAaaaaAAaaA"},
    {"K3", "(r'U'r)(R'U'RU)(r'Ur)", OLL, "aaaAAAaaA-AAaAaaaAaAaa"},
    {"K4", "(rUr')(RUR'U')(rU'r')", OLL, "aaAAAAaaa-aAaaaAaAAaaA"},

    {"A1", "y(RUR'U')(RU'R')(F'U'F)(RUR')", OLL, "AaAAAaaAa-aAaaAAaaaAaa"},
    {"A2", "y'FU(RU2R'U')(RU2R'U')F'", OLL, "AAaaAAAaa-aaAaaaAAaaAa"},
    {"A3", "(RUR'URU2R')F(RUR'U')F'", OLL, "aAaAAaAaA-AaAaAaaAaaaa"},
    {"A4", "(R'U'RU'R'U2R)F(RUR'U')F'", OLL, "AaAAAaaAa-aAaaAaAaAaaa"},

    {"L1", "F'(L'U'LU)(L'U'LU)F", OLL, "aAaaAAaaa-AaaAaAaAAaAa"},
    {"L2", "F(RUR'U')(RUR'U')F'", OLL, "aAaAAaaaa-aaAaAaAAaAaA"},
    {"L3", "rU'r2Ur2Ur2U'r", OLL, "aAaaAAaaa-aaAaaaAAaAAA"},
    {"L4", "r'Ur2U'r2U'r2Ur'", OLL, "aaaaAAaAa-aAAaaaAaaAAA"},
    {"L5", "(r'U'RU')(R'URU')R'U2r", OLL, "aaaaAAaAa-aAaAaAaaaAAA"},
    {"L6", "(rUR'U)(RU'R'U)RU2r'", OLL, "aAaaAAaaa-aaaAaAaAaAAA"},

    {"B1", "(rUR'URU2r')", OLL, "aAaAAaAaa-AaaAAaAAaaaa"},
    {"B2", "(r'U'RU'R'U2r)", OLL, "AaaAAaaAa-aAAaAAaaAaaa"},
    {"B3", "r'(R2UR'URU2R')UM'", OLL, "aaaaAAAAa-AAaAaaAaaaAa"},
    {"B4", "M'(R'U'RU'R'U2R)U'M", OLL, "AAaaAAaaa-aaAaaAaAAaAa"},
    {"B5", "(LF')(L'U'LU)FU'L'", OLL, "aaAAAAAaa-AAaaaAaAaaaa"},
    {"B6", "(R'F)(RUR'U')F'UR", OLL, "AaaAAAaaA-aAAaaaaAaAaa"},

    {"O1", "(RU2)(R2FRF')U2(R'FRF')", OLL, "aaaaAaaaa-aAaAAAaAaAAA"},
    {"O2", "F(RUR'U')F'f(RUR'U')f'", OLL, "aaaaAaaaa-aAAaAaAAaAAA"},
    {"O3", "f(RUR'U')f'U'F(RUR'U')F'", OLL, "aaaaAaaaA-AAaAAaaAaAAa"},
    {"O4", "f(RUR'U')f'UF(RUR'U')F'", OLL, "aaAaAaaaa-aAaaAAaAAaAA"},
    {"O5", "(RUR'U)(R'FRF')U2(R'FRF')", OLL, "AaaaAaaaA-aAAaAaaAaAAa"},
    {"O6", "yRU2(R2FRF')U2M'(URU'r')", OLL, "AaAaAaaaa-aAaaAaAAAaAa"},
    {"O7", "MU(RUR'U')M'(R'FRF')", OLL, "AaAaAaaaa-aAaaAAaAaAAa"},
    {"O8", "MU(RUR'U')M2(URU'r')", OLL, "AaAaAaAaA-aAaaAaaAaaAa"},



    // PLL
    {"Ub-perm", "R2U(RUR'U')R'U'(R'UR')", PLL, "MMMNmNmnmnNn"},
    {"Ua-perm", "(RU'RU)RU(RU'R'U')R2", PLL, "MMMNnNmNmnmn"},
    {"Z-perm", "M2UM2UM'U2M2U2M'U2", PLL, "MNMnmnmnmNMN"},
    {"H-perm", "M2UM2U2M2UM2", PLL, "MmMNnNmMmnNn"},

    {"Aa-perm", "x(R'UR')D2(RU'R')D2R2x'", PLL, "MNMnMmNnnmmN"},
    {"Ab-perm", "xR2D2(RUR')D2(RU'R)x'", PLL, "NnMnmnmNNMMm"},
    {"E-perm", "x'(RU'R'D)(RUR'D')(RUR'D)(RU'R'D')x", PLL, "MNmnmNmnMNMn"},

    {"Ra-perm", "(RU'R'U')(RURD)(R'U'RD')(R'U2R')U'", PLL, "MNnmnMnmmNMN"},
    {"Rb-perm", "(R'U2RU2)R'F(RUR'U')R'F'R2U'", PLL, "NMnMmNmNmnnM"},
    {"Ja-perm", "(R'UL'U2)(RU'R'U2RLU'", PLL, "MmmNMMnnnmNN"},
    {"Jb-perm", "(RUR'F')(RUR'U')R'FR2U'R'U'", PLL, "NNMnnNMMnmmm"},
    {"T-perm", "(RUR'U')(R'FR2U')R'U'(RUR'F')", PLL, "MMNmnMNmmnNn"},
    {"F-perm", "(R'U'F')(RUR'U')(R'FR2U')(R'U'RU)(R'UR)", PLL, "MmNmNMNMmnnn"},

    {"V-perm", "(R'UR'U')y(R'F'R2U')(R'UR'F)RF", PLL, "MNmNmnmMMnnN"},
    {"Y-perm", "F(RU'R'U')(RUR'F')(RUR'U')(R'FRF')", PLL, "MnmNNnmMMnmN"},
    {"Na-perm", "(RUR'U)(RUR'F')(RUR'U')(R'FR2U')R'U2(RU'R')", PLL, "NNnMMmnnNmmM"},
    {"Nb-perm", "(R'URU')(R'F'U'F)(RUR'F)R'F'(RU'R)", PLL, "NnnMmmnNNmMM"},
    
    {"Ga-perm", "R2U(R'UR'U')(RU'R2)DU'(R'URD')U", PLL, "NnMnmNMMnmNm"},
    {"Gb-perm", "y'D(R'U'RU)D'(R2UR'U)(RU'RU')R2U'", PLL, "NnNMNnmmMnMm"},
    {"Gc-perm", "R2U'(RU'RU)(R'UR2D')(URU'R')DU'", PLL, "NMMnmNMNnmnm"},
    {"Gd-perm", "D'(RUR'U')D(R2U'RU')(R'UR'U)R2U", PLL, "NnMnNNMmnmMm"}
};