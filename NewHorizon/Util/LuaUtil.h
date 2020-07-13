#pragma once
#ifndef _LUA_UTIL_H_
#define _LUA_UTIL_H_
#include "../ThirdParty/lua/lua.hpp"

class LuaUtil {
public:
	static lua_State* getNewGameObjectEvon();
	static void luaEnvironmentRelease(lua_State* pState);

};

#endif