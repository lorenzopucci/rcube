/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include "text.hpp"
#include "cube.hpp"

struct Config
{
    unsigned int scramble_length = 15;
    unsigned int kociemba_threads = 4;
    unsigned int kociemba_timeout = 1;
};

struct GlfwUserPtrData
{
    Camera *camera;
    Text *text;
    Timer *timer;
    Cube *cube3d;

    std::function<void(rcube::Move)> moveCallback = NULL;

    Config config;
    
    bool MVPupdated = false; // false when MPV needs to be sent to the GPU
    bool cubeUpdated = false; // false when the cube needs to be re-rendered
    bool hasBeenScrambled = false;
};