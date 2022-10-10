/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <rcube.hpp>

rcube::Center::Center(const Color &color, const rcube::Orientation &o)
{
    this->color = color;
    this->orientation = o;
    this->location = rcube::Coordinates(o);
}