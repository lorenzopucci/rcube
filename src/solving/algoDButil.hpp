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

enum AlgoType
{
    OLL, PLL, GENERAL
};

struct AlgoDBItem
{
    std::string name;
    std::string algo;
    AlgoType type = AlgoType::GENERAL;
    std::string match = "";
};
