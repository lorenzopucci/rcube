/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <iostream>

extern "C"
{
    #include <lua.hpp>
}

#include <rcube.hpp>
#include "luaAPI.hpp"


void returnList(lua_State *L, const std::map<std::string, int> &data)
{
    lua_createtable(L, data.size(), 0);
    int newTable = lua_gettop(L);

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        lua_pushnumber(L, it->second);
        lua_setfield(L, -2, it->first.c_str());
    }
}

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

int RcubeLua::rotateTo(lua_State *L)
{
    std::string tc = lua_tostring(L, 1);
    std::string fc = lua_tostring(L, 2);

    try
    {
        _cube->rotateTo(static_cast<Color>(tc[0]), static_cast<Color>(fc[0]));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

int RcubeLua::find(lua_State *L)
{
    std::string stks = lua_tostring(L, 1);

    rcube::Coordinates result;

    switch (stks.length())
    {
        case 1:
            result = _cube->find(static_cast<Color>(stks[0])); break;
        case 2:
            result = _cube->find(static_cast<Color>(stks[0]),
                static_cast<Color>(stks[1])); break;
        case 3:
            result = _cube->find(static_cast<Color>(stks[0]),
                static_cast<Color>(stks[1]),
                static_cast<Color>(stks[2])); break;
    }

    std::map<std::string, int> toReturn = {{"x", result.x()},
        {"y", result.y()}, {"z", result.z()}};

    returnList(L, toReturn);
    return 1; 
}

int RcubeLua::display(lua_State *L)
{
    _cube->display();
    return 1;
}

int RcubeLua::faceMatches(lua_State *L)
{
    int axis = lua_tonumber(L, 1);
    int direction = lua_tonumber(L, 2);
    std::string expr = lua_tostring(L, 3);

    rcube::Orientation orient = {static_cast<Axis>(axis), direction};

    bool result = _cube->faceMatches(orient, expr);

    lua_pushboolean(L, result);
    return 1;
}

int RcubeLua::layerMatches(lua_State *L)
{
    int axis = lua_tonumber(L, 1);
    int direction = lua_tonumber(L, 2);
    std::string expr = lua_tostring(L, 3);

    rcube::Orientation orient = {static_cast<Axis>(axis), direction};

    bool result = _cube->layerMatches(orient, expr);

    lua_pushboolean(L, result);
    return 1;
}