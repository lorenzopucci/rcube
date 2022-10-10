/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <rcube.hpp>
#include <utility.hpp>

rcube::Coordinates2D::Coordinates2D()
    : coords({0, 0})
{
    return;
}

rcube::Coordinates2D::Coordinates2D(const int& x, const int& y)
    : coords({x, y})
{
    return;
}

rcube::Coordinates2D::Coordinates2D(const rcube::Coordinates& coords3D,
    const rcube::Orientation& o)
{
    coords[Axis::X] = coords3D.coords[(o.axis + 1) % 3];
    coords[Axis::Y] = coords3D.coords[(o.axis + 2) % 3];
}

bool rcube::Coordinates2D::operator<(const rcube::Coordinates2D& a) const
{
    return (coords[X] < a.coords[X]) ||
        (coords[X] == a.coords[X] && coords[Y] < a.coords[Y]);
}

bool rcube::Coordinates2D::operator==(const rcube::Coordinates2D& a) const
{
    return (coords[X] == a.coords[X]) && (coords[Y] == a.coords[Y]);
}