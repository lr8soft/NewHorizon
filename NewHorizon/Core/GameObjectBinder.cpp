#include "GameObjectBinder.h"
#include "GameObjectManager.h"
#include "../Util/LogUtil.hpp"
#include <functional>
#include <sstream>
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

int GameObjectBinder::luaAddNewInstance(lua_State * luaState)
{
	luaL_checktype(luaState, -1, LUA_TSTRING);
	const char* instanceTagName = lua_tostring(luaState, -1);
	

	luaL_checktype(luaState, -2, LUA_TSTRING);
	const char* originObjectName = lua_tostring(luaState, -2);

	std::stringstream ss;
	ss << originObjectName << " " << instanceTagName;

	LogUtil::printInfo(ss.str());

	GameObject **s = (GameObject**)lua_newuserdata(luaState, sizeof(GameObject*));//lua_newuserdata(luaState, sizeof(GameObject*));
	*s = GameObjectManager::getInstance()->addGameObjectInstance(originObjectName, instanceTagName);
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

	glm::vec3 rotation = (*object)->getTransform().rotation;
	lua_settop(luaState, 0);
	lua_pushnumber(luaState, rotation.x);
	lua_pushnumber(luaState, rotation.y);
	lua_pushnumber(luaState, rotation.z);
	return 3;
}

int GameObjectBinder::luaSetScale(lua_State * luaState)
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
	transform.scale = glm::vec3(x, y, z);
	(*object)->setTransform(transform);

	return 3;
}

int GameObjectBinder::luaGetScale(lua_State * luaState)
{
	GameObject **object = (GameObject**)luaL_checkudata(luaState, 1, "NewHorizon.GameObject");
	luaL_argcheck(luaState, object != NULL, 1, "invalid GameObject data");

	glm::vec3 scale = (*object)->getTransform().scale;
	lua_settop(luaState, 0);
	lua_pushnumber(luaState, scale.x);
	lua_pushnumber(luaState, scale.y);
	lua_pushnumber(luaState, scale.z);
	return 3;
}

int GameObjectBinder::luaGetTagName(lua_State * luaState)
{
	GameObject **object = (GameObject**)luaL_checkudata(luaState, 1, "NewHorizon.GameObject");
	luaL_argcheck(luaState, object != NULL, 1, "invalid GameObject data");

	lua_settop(luaState, 0);
	lua_pushstring(luaState, (*object)->getTagName().c_str());

	return 1;
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

int GameObjectBinder::luaSetDead(lua_State * luaState)
{
	GameObject **object = (GameObject**)luaL_checkudata(luaState, 1, "NewHorizon.GameObject");
	luaL_argcheck(luaState, object != NULL, 1, "invalid GameObject data");

	luaL_checktype(luaState, -1, LUA_TBOOLEAN);
	bool value = lua_toboolean(luaState, -1);

	(*object)->setDead(value);

	return 0;
}

int GameObjectBinder::luaOpenGameObject(lua_State * luaState)
{
	luaL_newmetatable(luaState, "NewHorizon.GameObject");
	/* metatable.__index = metatable */
	lua_pushvalue(luaState, -1);  /* duplicates the metatable */
	lua_setfield(luaState, -2, "__index");

	static const struct luaL_Reg objectlib_m[] = {
	{"setPosition", GameObjectBinder::luaSetPosition},
	{"setScale", GameObjectBinder::luaSetScale},
	{"setRotation", GameObjectBinder::luaSetRotation},
	{"setDead", GameObjectBinder::luaSetDead},

	{"getPosition", GameObjectBinder::luaGetPosition},
	{"getScale", GameObjectBinder::luaGetScale},
	{"getRotation", GameObjectBinder::luaGetRotation},
	{"getDeltaTime", GameObjectBinder::luaGetDeltaTime},
	{"getAccmulateTime", GameObjectBinder::luaGetAccmulateTime},
	{"getTagName", GameObjectBinder::luaGetTagName},
	
	{NULL, NULL}
	};

	static const struct luaL_Reg objectlib_f[] = {
	{"getObject", luaGetInstance},
	{"newInstance", luaAddNewInstance},
	{NULL, NULL}
	};

	luaL_setfuncs(luaState, objectlib_m, 0);
	luaL_newlib(luaState, objectlib_f);

	return 1;
}
