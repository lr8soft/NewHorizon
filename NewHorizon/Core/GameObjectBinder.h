#pragma once
#ifndef _GAME_OBJECT_BINDER_H_
#define _GAME_OBJECT_BINDER_H_
#include "../Util/LuaUtil.h"
#include "GameObject.h"
class GameObjectBinder {
private:
	static GameObject* pInstance;
public:
	static void setCurrentInstance(GameObject* pInstance);

	static int luaGetInstance(lua_State* luaState);

	static int luaAddNewInstance(lua_State* luaState);

	static int luaSetPosition(lua_State* luaState);
	static int luaSetRotation(lua_State* luaState);
	static int luaSetScale(lua_State* luaState);
	static int luaSetDead(lua_State* luaState);

	static int luaGetPosition(lua_State* luaState);
	static int luaGetRotation(lua_State* luaState);
	static int luaGetScale(lua_State* luaState);
	static int luaGetTagName(lua_State* luaState);

	static int luaGetDeltaTime(lua_State* luaState);
	static int luaGetAccmulateTime(lua_State* luaState);

	

	static int luaOpenGameObject(lua_State* luaState);
};
#endif