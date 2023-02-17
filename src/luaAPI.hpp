/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#ifndef IGNORE_LUA

extern "C"
{
    #include <lua.hpp>
}

class RcubeLua
{
public:
    RcubeLua(rcube::Cube *cube);

    int scramble(lua_State *L);

    int performMove(lua_State *L);

    int performAlgorithm(lua_State *L);

    int isSolved(lua_State *L);

    int rotateTo(lua_State *L);

    int find(lua_State *L);

    int getStickerOrientation(lua_State *L);

    int getStickerAt(lua_State *L);

    int getFaceColor(lua_State *L);

    int getFaceOrientation(lua_State *L);

    int display(lua_State *L);

    int faceMatches(lua_State *L);

    int layerMatches(lua_State *L);

    int normalizeAlgo(lua_State *L);

    int reverseAlgo(lua_State *L);

    int removeRotations(lua_State *L);

    int solveCfop(lua_State *L);

    rcube::Cube *_cube;
};

#endif