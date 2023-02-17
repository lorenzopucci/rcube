/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#ifndef IGNORE_LUA

#include <iostream>
#include <string>
#include <map>

extern "C"
{
    #include <lua.hpp>
}

#include <rcube.hpp>
#include "luaAPI.hpp"


void pushTable(lua_State *L, const std::map<std::string, int> &data)
{
    lua_createtable(L, data.size(), 0);
    int newTable = lua_gettop(L);

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        lua_pushnumber(L, it->second);
        lua_setfield(L, -2, it->first.c_str());
    }
}

void loadTable(lua_State *L, std::map<std::string, int> *data, int stackIdx = 1)
{
    if (!lua_istable(L, stackIdx)) return;

    for (auto it = data->begin(); it != data->end(); ++it)
    {
        lua_pushstring(L, it->first.c_str());
        lua_gettable(L, stackIdx);
        it->second = lua_tointeger(L, -1);
        lua_pop(L, 1);
    }
    return;
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
    _cube->performAlgorithm(lua_tostring(L, 1));
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

    pushTable(L, toReturn);
    return 1; 
}

int RcubeLua::getStickerOrientation(lua_State *L)
{
    std::map<std::string, int> coords = {{"x", 0}, {"y", 0}, {"z", 0}};
    loadTable(L, &coords);
    rcube::Coordinates coordinates(coords["x"], coords["y"], coords["z"]);

    Color color = static_cast<Color>((lua_tostring(L, 2))[0]);

    rcube::Orientation orient = _cube->getStickerOrientation(coordinates,
        color);

    std::map<std::string, int> toReturn = {{"axis", orient.axis},
        {"direction", orient.direction}};
    pushTable(L, toReturn);
    
    return 1;
}

int RcubeLua::getStickerAt(lua_State *L)
{
    std::map<std::string, int> coords = {{"x", 0}, {"y", 0}, {"z", 0}};
    loadTable(L, &coords, 1);
    rcube::Coordinates coordinates(coords["x"], coords["y"], coords["z"]);

    std::map<std::string, int> orient = {{"axis", 0}, {"direction", 0}};
    loadTable(L, &orient, 2);
    rcube::Orientation orientation = {(Axis)orient["axis"],
        orient["direction"]};

    char res[2] = {(char)_cube->getStickerAt(coordinates, orientation), '\0'};
    lua_pushstring(L, res);
    return 1;
}

int RcubeLua::getFaceColor(lua_State *L)
{
    std::map<std::string, int> orient = {{"axis", 0}, {"direction", 0}};
    loadTable(L, &orient);
    rcube::Orientation orientation = {(Axis)orient["axis"],
        orient["direction"]};

    char res[2] = {(char)_cube->getFaceColor(orientation), '\0'};
    lua_pushstring(L, res);
    return 1;
}

int RcubeLua::getFaceOrientation(lua_State *L)
{
    Color col = (Color)lua_tostring(L, 1)[0];
    rcube::Orientation ort = _cube->getFaceOrientation(col);
    std::map<std::string, int> orient = {{"axis", (int)ort.axis},
        {"direction", ort.direction}};
    pushTable(L, orient);
    return 1;
}

int RcubeLua::display(lua_State *L)
{
    _cube->display();
    return 1;
}

int RcubeLua::faceMatches(lua_State *L)
{
    std::map<std::string, int> orient = {{"axis", 0}, {"direction", 0}};
    loadTable(L, &orient);
    rcube::Orientation orientation = {(Axis)orient["axis"],
        orient["direction"]};

    std::string expr = lua_tostring(L, 2);

    std::map<std::string, int> destTmp = {{"x", 0}, {"y", 0}, {"z", 0}};
    loadTable(L, &destTmp, 3);
    rcube::Coordinates dest(destTmp["x"], destTmp["y"], destTmp["z"]);

    lua_pushboolean(L, _cube->faceMatches(orientation, expr, dest));
    return 1;
}

int RcubeLua::layerMatches(lua_State *L)
{
    std::map<std::string, int> layer = {{"axis", 0}, {"direction", 0}};
    loadTable(L, &layer);
    rcube::Orientation lOrient = {(Axis)layer["axis"], layer["direction"]};

    std::string expr = lua_tostring(L, 2);

    std::map<std::string, int> destTmp = {{"x", 0}, {"y", 0}, {"z", 0}};
    loadTable(L, &destTmp, 3);
    rcube::Coordinates dest(destTmp["x"], destTmp["y"], destTmp["z"]);

    std::map<std::string, int> orient = {{"axis", 0}, {"direction", 0}};
    loadTable(L, &orient, 4);
    rcube::Orientation dOrient = {(Axis)orient["axis"], orient["direction"]};

    lua_pushboolean(L, _cube->layerMatches(lOrient, expr, dest, dOrient));
    return 1;
}

int RcubeLua::normalizeAlgo(lua_State *L)
{
    rcube::Algorithm algo(lua_tostring(L, 1));
    algo.normalize();

    lua_pushstring(L, algo.to_string().c_str());
    return 1;
}

int RcubeLua::reverseAlgo(lua_State *L)
{
    rcube::Algorithm algo(lua_tostring(L, 1));
    algo = algo.reverse();
    
    lua_pushstring(L, algo.to_string().c_str());
    return 1;
}

int RcubeLua::removeRotations(lua_State *L)
{
    rcube::Algorithm algo(lua_tostring(L, 1));
    algo.removeRotations();

    lua_pushstring(L, algo.to_string().c_str());
    return 1;
}

int RcubeLua::solveCfop(lua_State *L)
{
    rcube::Algorithm solution = _cube->solveCfop();
    lua_pushstring(L, solution.to_string().c_str());
    return 1;
}

#endif