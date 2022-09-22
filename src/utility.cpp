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

    for (int i = 0; i < 3; ++i)
    {
        // new axis is neither rotation nor current
        if (i != rotAxis && i != axis)
        {
            axis = static_cast<Axis>(i);
            break;
        }
    }

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
            coords[Axis::Y] = coords[Axis::Y] * cos - coords[Axis::Z] * sin;
            coords[Axis::Z] = y0 * sin + coords[Axis::Z] * cos;
            break;
        }
        case Axis::Y:
        {
            int x0 = coords[Axis::X];
            coords[Axis::X] = coords[Axis::X] * cos + coords[Axis::Z] * sin;
            coords[Axis::Z] = coords[Axis::Y] * cos - x0 * sin;
            break;
        }
        case Axis::Z:
        {
            int x0 = coords[Axis::X];
            coords[Axis::X] = coords[Axis::X] * cos - coords[Axis::Y] * sin;
            coords[Axis::Y] = x0 * sin + coords[Axis::Y] * cos;
            break;
        }
    }
}

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