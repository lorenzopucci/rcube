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
#include <functional>

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#include <rcube.hpp>
#include "luaAPI.hpp"

bool checkStatus(lua_State *L, int r)
{
    if (r == LUA_OK) return true;

    std::string msg = lua_tostring(L, -1);
    std::cerr << msg << std::endl;
    return false;
}

typedef int (RcubeLua::*memberFunc)(lua_State *L);

template <memberFunc func>
int runFunction(lua_State *L) {
    RcubeLua *instance = *static_cast<RcubeLua**>(lua_getextraspace(L));
    return (instance->*func)(L);
}

void initLua(lua_State *L)
{
    luaL_openlibs(L);

    lua_register(L, "scramble", &runFunction<&RcubeLua::scramble>);
    lua_register(L, "performMove", &runFunction<&RcubeLua::performMove>);
    lua_register(L, "performAlgorithm",
        &runFunction<&RcubeLua::performAlgorithm>);
    lua_register(L, "isSolved", &runFunction<&RcubeLua::isSolved>);
    lua_register(L, "rotateTo", &runFunction<&RcubeLua::rotateTo>);
    lua_register(L, "find", &runFunction<&RcubeLua::find>);
    lua_register(L, "getStickerOrientation",
        &runFunction<&RcubeLua::getStickerOrientation>);
    lua_register(L, "getStickerAt", &runFunction<&RcubeLua::getStickerAt>);
    lua_register(L, "getFaceColor", &runFunction<&RcubeLua::getFaceColor>);
    lua_register(L, "getFaceOrientation", &runFunction<&RcubeLua::getFaceOrientation>);
    lua_register(L, "display", &runFunction<&RcubeLua::display>);
    lua_register(L, "faceMatches", &runFunction<&RcubeLua::faceMatches>);
    lua_register(L, "layerMatches", &runFunction<&RcubeLua::layerMatches>);
    lua_register(L, "layerAndFaceMatch", &runFunction<&RcubeLua::layerAndFaceMatch>);
    lua_register(L, "normalizeAlgo", &runFunction<&RcubeLua::normalizeAlgo>);
    lua_register(L, "reverseAlgo", &runFunction<&RcubeLua::reverseAlgo>);
    lua_register(L, "removeRotations", &runFunction<&RcubeLua::removeRotations>);
    lua_register(L, "solveCfop", &runFunction<&RcubeLua::solveCfop>);
    lua_register(L, "solveKociemba", &runFunction<&RcubeLua::solveKociemba>);
}

int rcube::Cube::runScript(const std::string &path)
{
    lua_State *L = luaL_newstate();
    initLua(L);

    RcubeLua instance(this);
    *static_cast<RcubeLua**>(lua_getextraspace(L)) = &instance;

    int status = checkStatus(L, luaL_dofile(L, path.c_str()));

    lua_close(L);
    return status;
}

int rcube::Cube::runCommand(const std::string &cmd)
{
    lua_State *L = luaL_newstate();
    initLua(L);

    RcubeLua instance(this);
    *static_cast<RcubeLua**>(lua_getextraspace(L)) = &instance;

    int status = checkStatus(L, luaL_dostring(L, cmd.c_str()));

    lua_close(L);
    return status;
}

#endif