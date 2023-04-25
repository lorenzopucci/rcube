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

rcube::Coordinates::Coordinates(const rcube::Orientation& o1)
{
    coords[o1.axis] = o1.direction;
    coords[(o1.axis + 1) % 3] = 0;
    coords[(o1.axis + 2) % 3] = 0;
}

rcube::Coordinates::Coordinates(const int& x, const int& y, const int& z)
    : coords({x, y, z})
{}

rcube::Coordinates::Coordinates(const rcube::Orientation& o1,
    const rcube::Orientation& o2)
{
    coords[o1.axis] = o1.direction;
    coords[o2.axis] = o2.direction;

    for (int i = 0; i < 3; ++i)
    {
        if (i != o1.axis && i != o2.axis)
        {
            coords[i] = 0;
        }
    }
}

rcube::Coordinates::Coordinates(const rcube::Orientation& o1,
    const rcube::Orientation& o2, const rcube::Orientation& o3)
{
    coords[o1.axis] = o1.direction;
    coords[o2.axis] = o2.direction;
    coords[o3.axis] = o3.direction;
}

bool rcube::Coordinates::operator==(const rcube::Coordinates &a) const
{
    for (int i = 0; i < 3; ++i)
    {
        if (coords[i] != a.coords[i]) return false;
    }
    return true;
}

bool rcube::Coordinates::operator!=(const rcube::Coordinates &a) const
{
    return (x() != a.x() || y() != a.y() || z() != a.z());
}

std::string rcube::Coordinates::to_string() const
{
    return "(" + std::to_string(x()) + ", " + std::to_string(y()) + ", " +
        std::to_string(z()) + ")";
}

void rcube::Coordinates::rotate (const Axis& axis, int step)
{
    step = ((-step % 4) + 4) % 4; // sanitize input
    int vals[] = {0, 1, 0, -1};
    int sin = vals[step];
    int cos = vals[(step + 1) % 4];

    switch (axis)
    {
        case Axis::X:
        {
            int y0 = coords[Axis::Y];
            coords[Axis::Y] = y0 * cos - coords[Axis::Z] * sin;
            coords[Axis::Z] = y0 * sin + coords[Axis::Z] * cos;
            break;
        }
        case Axis::Y:
        {
            int x0 = coords[Axis::X];
            coords[Axis::X] =  x0 * cos + coords[Axis::Z] * sin;
            coords[Axis::Z] = -x0 * sin + coords[Axis::Z] * cos;
            break;
        }
        case Axis::Z:
        {
            int x0 = coords[Axis::X];
            coords[Axis::X] = x0 * cos - coords[Axis::Y] * sin;
            coords[Axis::Y] = x0 * sin + coords[Axis::Y] * cos;
            break;
        }
    }
}

rcube::Coordinates rcube::Coordinates::getRotated(const Axis& axis, int step)
    const
{
    rcube::Coordinates newCoords(x(), y(), z());
    newCoords.rotate(axis, step);
    
    return newCoords;
}