#include "LuaUtil.h"
lua_State* LuaUtil::luaEnvironmentInit()
{
	lua_State* luaState = luaL_newstate();
	static const luaL_Reg lualibs[] = {
		{ "base", luaopen_base },
		{"io", luaopen_io },
		{ NULL, NULL}
	};

	const luaL_Reg *lib = lualibs;
	for (; lib->func != nullptr; lib++)
	{
		luaL_requiref(luaState, lib->name, lib->func, 1);
		lua_pop(luaState, 1);
	}
	return luaState;
}

void LuaUtil::luaEnvironmentRelease(lua_State* pState)
{
	lua_close(pState);
}