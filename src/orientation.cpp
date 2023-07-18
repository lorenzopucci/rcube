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

bool rcube::Orientation::operator<(const rcube::Orientation& a) const
{
    return (axis < a.axis) || (axis == a.axis && direction < a.direction);
}

bool rcube::Orientation::operator==(const rcube::Orientation& a) const
{
    return (axis == a.axis) && (direction == a.direction);
}

bool rcube::Orientation::operator!=(const rcube::Orientation& a) const
{
    return (axis != a.axis) || (direction != a.direction);
}

void rcube::Orientation::rotate(const Axis& rotAxis, int step)
{
    // reduce input to {-2, -1, 0, 1, 2}
    step = step % 4;
    if (abs(step) == 3) step /= -abs(step);

    Axis lastAxis = axis;

    if (rotAxis == axis || step == 0) return;

    if (abs(step) == 2)
    {
        direction *= -1;
        return;
    }

    // new axis is neither rotAxis nor old axis
    axis = static_cast<Axis>(3 - (rotAxis + axis));


    if (step == 1)
    {
        // direction changes happen to occur when the difference between the
        // indices of axis and lastAxis is 1
        if ((axis - lastAxis + 3) % 3 == 1) direction *= -1;
        return;
    }
    else if (step == -1)
    {
        // direction changes happen to occur when the difference between the
        // indices of axis and lastAxis is -1
        if ((axis - lastAxis + 3) % 3 == 2) direction *= -1;
    }
}

rcube::Orientation rcube::Orientation::getRotated(const Axis& rotAxis,
    int step) const
{
    rcube::Orientation newOrient = {axis, direction};
    newOrient.rotate(rotAxis, step);
    
    return newOrient;
}

rcube::Orientation rcube::Orientation::getInv() const
{
    return rcube::Orientation{axis, -direction};
}

std::vector<rcube::Orientation> rcube::Orientation::iterate()
{
    std::vector<rcube::Orientation> vec;
    for (int a = 0, d = -1; a < 3; d *= -1)
    {
        vec.push_back({static_cast<Axis>(a), d});
        if (d == 1) a++;
    }
    return vec;
}

std::vector<rcube::Orientation> rcube::Orientation::iterateOnBlock(const
    rcube::Coordinates &block)
{
    std::vector<rcube::Orientation> res;

    for (int i = 0; i < 3; ++i)
    {
        if (block.coords[i] != 0)
            res.push_back((rcube::Orientation){(Axis)i, block.coords[i]});
    }
    return res;
}