/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

extern "C"
{
    #include <lua.hpp>
}

#include <rcube.hpp>
#include "luaAPI.hpp"

RcubeLua::RcubeLua(rcube::Cube *cube)
{
    _cube = cube;
}

int RcubeLua::scramble(lua_State *L)
{
    int len = (int)lua_tonumber(L, 1);

    rcube::Algorithm algo;
    _cube->scramble(len, &algo);

    lua_pushstring(L, algo.to_string().c_str());
    return 1;
}

int RcubeLua::performMove(lua_State *L)
{
    std::string move = lua_tostring(L, 1);
    int direction = (int)lua_tonumber(L, 2);

    _cube->performMove(rcube::Move(move[0], direction));
    return 1;
}

int RcubeLua::performAlgorithm(lua_State *L)
{
    std::string algo = lua_tostring(L, 1);

    _cube->performAlgorithm(rcube::Algorithm(algo));
    return 1;
}

int RcubeLua::isSolved(lua_State *L)
{
    lua_pushboolean(L, _cube->isSolved());
    return 1;
}

int RcubeLua::display(lua_State *L)
{
    _cube->display();
    return 1;
}

int RcubeLua::matches(lua_State *L)
{
    int axis = lua_tonumber(L, 1);
    int direction = lua_tonumber(L, 2);
    std::string expr = lua_tostring(L, 3);

    rcube::Orientation orient = {static_cast<Axis>(axis), direction};

    bool result = _cube->matches(orient, expr);

    lua_pushboolean(L, result);
    return 1;
}