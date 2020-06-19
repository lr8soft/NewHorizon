#include "GameObjectBinder.h"
#include "../Util/LogUtil.hpp"
#include <functional>
GameObject* GameObjectBinder::pInstance = nullptr;
void GameObjectBinder::setCurrentInstance(GameObject * instance)
{
	GameObjectBinder::pInstance = instance;
}
int GameObjectBinder::luaGetInstance(lua_State * luaState)
{
	GameObject **s = (GameObject**)lua_newuserdata(luaState, sizeof(GameObject*));
	*s = GameObjectBinder::pInstance;

	luaL_getmetatable(luaState, "NewHorizon.GameObject");
	lua_setmetatable(luaState, -2);

	return 1;
}

int GameObjectBinder::luaSetPosition(lua_State * luaState)
{
	GameObject **object = (GameObject**)luaL_checkudata(luaState, 1, "NewHorizon.GameObject");
	luaL_argcheck(luaState, object != NULL, 1, "invalid GameObject data");

	luaL_checktype(luaState, -1, LUA_TNUMBER);
	float z = lua_tonumber(luaState, -1);
	//lua_pop(luaState, -1);

	luaL_checktype(luaState, -2, LUA_TNUMBER);
	float y = lua_tonumber(luaState, -2);
	//lua_pop(luaState, -1);

	luaL_checktype(luaState, -3, LUA_TNUMBER);
	float x = lua_tonumber(luaState, -3);
	//lua_pop(luaState, -1);
	Transform transform = (*object)->getTransform();
	transform.position = glm::vec3(x, y, z);
	(*object)->setTransform(transform);

	return 0;
}

int GameObjectBinder::luaGetPosition(lua_State * luaState)
{
	GameObject **object = (GameObject**)luaL_checkudata(luaState, 1, "NewHorizon.GameObject");
	luaL_argcheck(luaState, object != NULL, 1, "invalid GameObject data");

	glm::vec3 position = (*object)->getTransform().position;
	lua_settop(luaState, 0);
	lua_pushnumber(luaState, position.x);
	lua_pushnumber(luaState, position.y);
	lua_pushnumber(luaState, position.z);
	return 3;
}

int GameObjectBinder::luaSetRotation(lua_State * luaState)
{
	GameObject **object = (GameObject**)luaL_checkudata(luaState, 1, "NewHorizon.GameObject");
	luaL_argcheck(luaState, object != NULL, 1, "invalid GameObject data");

	luaL_checktype(luaState, -1, LUA_TNUMBER);
	float z = lua_tonumber(luaState, -1);
	//lua_pop(luaState, -1);

	luaL_checktype(luaState, -2, LUA_TNUMBER);
	float y = lua_tonumber(luaState, -2);
	//lua_pop(luaState, -1);

	luaL_checktype(luaState, -3, LUA_TNUMBER);
	float x = lua_tonumber(luaState, -3);
	//lua_pop(luaState, -1);
	Transform transform = (*object)->getTransform();
	transform.rotation = glm::vec3(x, y, z);
	(*object)->setTransform(transform);

	return 0;
}

int GameObjectBinder::luaGetRotation(lua_State * luaState)
{
	GameObject **object = (GameObject**)luaL_checkudata(luaState, 1, "NewHorizon.GameObject");
	luaL_argcheck(luaState, object != NULL, 1, "invalid GameObject data");

	glm::vec3 position = (*object)->getTransform().rotation;
	lua_settop(luaState, 0);
	lua_pushnumber(luaState, position.x);
	lua_pushnumber(luaState, position.y);
	lua_pushnumber(luaState, position.z);
	return 3;
}

int GameObjectBinder::luaGetDeltaTime(lua_State * luaState)
{
	GameObject **object = (GameObject**)luaL_checkudata(luaState, 1, "NewHorizon.GameObject");
	luaL_argcheck(luaState, object != NULL, 1, "invalid GameObject data");

	lua_settop(luaState, 0);
	lua_pushnumber(luaState, (*object)->getDeltaTime());
	return 1;
}

int GameObjectBinder::luaGetAccmulateTime(lua_State * luaState)
{
	GameObject **object = (GameObject**)luaL_checkudata(luaState, 1, "NewHorizon.GameObject");
	luaL_argcheck(luaState, object != NULL, 1, "invalid GameObject data");

	lua_settop(luaState, 0);
	lua_pushnumber(luaState, (*object)->getAccmulateTime());
	return 1;
}

int GameObjectBinder::luaOpenGameObject(lua_State * luaState)
{
	luaL_newmetatable(luaState, "NewHorizon.GameObject");
	/* metatable.__index = metatable */
	lua_pushvalue(luaState, -1);  /* duplicates the metatable */
	lua_setfield(luaState, -2, "__index");

	static const struct luaL_Reg objectlib_m[] = {
	{"setPosition", GameObjectBinder::luaSetPosition},
	{"getPosition", GameObjectBinder::luaGetPosition},
	{"setRotation", GameObjectBinder::luaSetRotation},
	{"getRotation", GameObjectBinder::luaGetRotation},

	{"getDeltaTime", GameObjectBinder::luaGetDeltaTime},
	{"getAccmulateTime", GameObjectBinder::luaGetAccmulateTime},
	{NULL, NULL}
	};

	static const struct luaL_Reg objectlib_f[] = {
	{"getObject", luaGetInstance},
	{NULL, NULL}
	};

	//luaL_setfuncs(luaState, objectlib_m, 0);

	luaL_setfuncs(luaState, objectlib_m, 0);
	luaL_newlib(luaState, objectlib_f);

	return 1;
}
