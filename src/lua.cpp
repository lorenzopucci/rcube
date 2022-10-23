/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

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
    return ((*instance).*func)(L);
}

int rcube::Cube::runScript(const std::string &path)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    RcubeLua instance(this);
    *static_cast<RcubeLua**>(lua_getextraspace(L)) = &instance;

    lua_register(L, "scramble", &runFunction<&RcubeLua::scramble>);
    lua_register(L, "performMove", &runFunction<&RcubeLua::performMove>);
    lua_register(L, "performAlgorithm",
        &runFunction<&RcubeLua::performAlgorithm>);
    lua_register(L, "isSolved", &runFunction<&RcubeLua::isSolved>);
    lua_register(L, "display", &runFunction<&RcubeLua::display>);

    int status = checkStatus(L, luaL_dofile(L, "scripts/test.lua"));

    lua_close(L);
    return status;
}