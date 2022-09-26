#include <rcube.hpp>

rcube::Center::Center(const Color &color, const rcube::Orientation &o)
{
    this->color = color;
    this->orientation = o;
    this->location = rcube::Coordinates(o);
}