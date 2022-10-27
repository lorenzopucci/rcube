/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <map>
#include <string>

std::map<std::string, std::string> algoDb
{
    // general
    {"sexy-move", "RUR'U'"},
    {"inv-sexy-move", "RUR'U'"},
    {"sune", "RUR'UR2UR'"},
    {"anti-sune", "L'U'LU'L'2U'L"},


    // OLL
    {"OCLL1", "(RU2R')(U'RUR')(U'RU'R')"},
    {"OCLL2", "RU2'R2'U'R2U'R2'U2'R"},
    {"OCLL3", "R2D(R'U2R)D'(R'U2R')"},
    {"OCLL4", "(rUR'U')(r'FRF')"},
    {"OCLL5", "yF'(rUR'U')r'FR"},
    {"OCLL6", "RU2R'U'RU'R'"},
    {"OCLL7", "RUR'URU2'R'"},

    {"T1", "(RUR'U')(R'FRF')"},
    {"T2", "F(RUR'U')F'"},

    {"S1", "(r'U2'RUR'Ur)"},
    {"S2", "(rU2R'U'RU'r')"},

    {"C1", "(RUR2'U')(R'FRU)RU'F'"},
    {"C2", "R'U'(R'FRF')UR"},

    {"W1", "(R'U'RU')(R'URU)lU'R'Ux"},
    {"W2", "(RUR'U)(RU'R'U')(R'FRF')"},

    {"E1", "(rUR'U')M(URU'R')"},
    {"E2", "(RUR'U')M'(URU'r')"},

    {"P1", "(R'U'F)(URU'R')F'R"},
    {"P2", "RUB'(U'R'U)(RBR')"},
    {"P3", "yR'U'F'UFR"},
    {"P4", "f(RUR'U')f'"},

    {"I1", "f(RUR'U')(RUR'U')f'"},
    {"I2", "(R'U'RU'R'U)y'(R'UR)B"},
    {"I3", "y(R'FRU)(RU'R2'F')R2U'R'(URUR')"},
    {"I4", "r'U'r(U'R'UR)(U'R'UR)r'Ur"},

    {"F1", "(RUR'U')R'F(R2UR'U')F'"},
    {"F2", "(RUR'U)(R'FRF')(RU2'R')"},
    {"F3", "(RU2')(R2'FRF')(RU2'R')"},
    {"F4", "F(RU'R'U')(RUR'F')"},

    {"K1", "(rU'r')(U'rUr')y'(R'UR)"},
    {"K2", "(R'FR)(UR'F'R)(FU'F')"},
    {"K3", "(r'U'r)(R'U'RU)(r'Ur)"},
    {"K4", "(rUr')(RUR'U')(rU'r')"},

    {"A1", "y(RUR'U')(RU'R')(F'U'F)(RUR')"},
    {"A2", "y'FU(RU2R'U')(RU2R'U')F'"},
    {"A3", "(RUR'URU2'R')F(RUR'U')F'"},
    {"A4", "(R'U'RU'R'U2R)F(RUR'U')F'"},

    {"L1", "F'(L'U'LU)(L'U'LU)F"},
    {"L2", "F(RUR'U')(RUR'U')F'"},
    {"L3", "rU'r2'Ur2Ur2'U'r"},
    {"L4", "r'Ur2U'r2'U'r2Ur'"},
    {"L5", "(r'U'RU')(R'URU')R'U2r"},
    {"L6", "(rUR'U)(RU'R'U)RU2'r'"},

    {"B1", "(rUR'URU2'r')"},
    {"B2", "(r'U'RU'R'U2r)"},
    {"B3", "r'(R2UR'URU2R')UM'"},
    {"B4", "M'(R'U'RU'R'U2R)U'M"},
    {"B5", "(LF')(L'U'LU)FU'L'"},
    {"B6", "(R'F)(RUR'U')F'UR"},

    {"O1", "(RU2')(R2'FRF')U2'(R'FRF')"},
    {"O2", "F(RUR'U')F'f(RUR'U')f'"},
    {"O3", "f(RUR'U')f'U'F(RUR'U')F'"},
    {"O4", "f(RUR'U')f'UF(RUR'U')F'"},
    {"O5", "(RUR'U)(R'FRF')U2'(R'FRF')"},
    {"O6", "yRU2'(R2'FRF')U2'M'(URU'r')"},
    {"O7", "MU(RUR'U')M'(R'FRF')"},
    {"O8", "MU(RUR'U')M2'(URU'r')"},



    // PLL
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
    {"F-perm", "(R'U'F')(RUR'U')(R'FR2U')(R'U'RU)(R'UR)"},

    {"V-perm", "(R'UR'U')y(R'F'R2U')(R'UR'F)RF"},
    {"Y-perm", "F(RU'R'U')(RUR'F')(RUR'U')(R'FRF')"},
    {"Na-perm", "(RUR'U)(RUR'F')(RUR'U')(R'FR2U')R'U2(RU'R')"},
    {"Nb-perm", "(R'URU')(R'F'U'F)(RUR'F)R'F'(RU'R)"},
    
    {"Ga-perm", "R2U(R'UR'U')(RU'R2)DU'(R'URD')U"},
    {"Gb-perm", "y'D(R'U'RU)D'(R2UR'U)(RU'RU')R2'U'"},
    {"Gc-perm", "R2U'(RU'RU)(R'UR2D')(URU'R')DU'"},
    {"Gd-perm", "D'(RUR'U')D(R2U'RU')(R'UR'U)R2U"}
};