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

#include <rcube.hpp>
#include <utility.hpp>

std::vector<Color> getAdjacentColors(const Color &color);

Color getOppositeColor(const Color &color);

rcube::Coordinates getBlockPlace(rcube::Cube *cube, const Color &c1,
    const Color &c2);
    
rcube::Coordinates getBlockPlace(rcube::Cube *cube, const Color &c1,
    const Color &c2, const Color &c3);