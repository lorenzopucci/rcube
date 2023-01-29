#pragma once

#include <rcube.hpp>

#include <functional>

namespace rcubeUI
{
    // everytime the user inputs a move, it is sent to moveCallback
    int runUI(rcube::Cube *cube, std::function<void(rcube::Move)>
        moveCallback = NULL);
}