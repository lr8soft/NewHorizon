#include "LuaUtil.h"
#include "LogUtil.hpp"

#include "../Core/GameObjectBinder.h"
lua_State* LuaUtil::luaEnvironmentInit()
{
	lua_State* luaState = luaL_newstate();
	static const luaL_Reg lualibs[] = {
		{ "base", luaopen_base },
		{ "math", luaopen_math },
		{ "io", luaopen_io },
		{ "NewHorizon", GameObjectBinder::luaOpenGameObject},
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



/*
	lua_State* luaState = LuaUtil::luaEnvironmentInit();

	int status = luaL_loadfile(luaState, "assets/Script/test.lua");
	{//call add(1, 233.0)
		lua_pcall(luaState, 0, LUA_MULTRET, 0);

		lua_getglobal(luaState, "add");
		lua_pushnumber(luaState, 1);
		lua_pushnumber(luaState, 233.0);

		lua_call(luaState, 2, 1);//2 parameter, 1 return
		float sum = lua_tonumber(luaState, -1);// get return from -1 (top of stack)
		lua_pop(luaState, 1);//pop the return value

		LogUtil::printInfo(std::to_string(sum));
	}
	{//globalTestValue = "LT_lrsoft"
		lua_pushstring(luaState, "LT_lrsoft");
		lua_setglobal(luaState, "globalTestValue");
	}
	{//read from globalTestValue
		lua_getglobal(luaState, "globalTestValue");
		string str = lua_tostring(luaState, -1);
		lua_pop(luaState, 1);
		LogUtil::printInfo(str);
	}
	{//send luatable to lua file
		lua_createtable(luaState, 2, 0);
		lua_pushnumber(luaState, 1);
		lua_pushnumber(luaState, 233);

		lua_rawset(luaState, -3);
		lua_pushnumber(luaState, 2);
		lua_pushstring(luaState, "testtesttest");

		lua_rawset(luaState, -3);
		lua_pushnumber(luaState, 3);
		lua_pushstring(luaState, "lualualua");

		lua_rawset(luaState, -3);
		lua_pushnumber(luaState, 3);
		lua_pushstring(luaState, "aaaaaa");

		lua_rawset(luaState, -3);
		lua_setglobal(luaState, "arg");
		luaL_dofile(luaState, "assets/Script/test.lua");
	}
	{//get multi-return from lua
		lua_pcall(luaState, 0, LUA_MULTRET, 0);
		lua_getglobal(luaState, "getValue");
		lua_call(luaState, 0, 3);

		while(lua_gettop(luaState))
		{
			switch (lua_type(luaState, lua_gettop(luaState)))
			{
			case LUA_TNUMBER:
				LogUtil::printInfo(std::to_string(lua_tonumber(luaState, lua_gettop(luaState)))); break;
			case LUA_TTABLE:
				LogUtil::printInfo("TYPE TABLE"); break;
			case LUA_TSTRING:
				LogUtil::printInfo(lua_tostring(luaState, lua_gettop(luaState))); break;
			case LUA_TBOOLEAN:
				LogUtil::printInfo(std::to_string(lua_toboolean(luaState, lua_gettop(luaState)))); break;
			default:
				LogUtil::printError("Unknown return type");
			}
			lua_pop(luaState, 1);
		}

	}
*/