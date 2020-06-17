#include "GameObjectManager.h"
#include "../Util/JsonLoader.h"
#include "../Util/LogUtil.hpp"
#include "../HorizonFrame.h"
#include <thread>

#include "../Util/LuaUtil.h"
using namespace std;
GameObjectManager* GameObjectManager::pInstance = nullptr;

GameObjectManager::GameObjectManager(){}
GameObjectManager * GameObjectManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new GameObjectManager;
	}
	return pInstance;
}

void GameObjectManager::onLogicalInit()
{
	auto configJson = JsonLoader::getJsonFromFile("assets/Config/Instance.json");
	auto objectDeclareValue = (*configJson)["objectDeclare"]["object"];
	auto objectInstanceValue = (*configJson)["objectInstance"];

	map<string, string> declareObject;//map of declare files
	for (int i = 0; i< objectDeclareValue.size(); i++)//Load all declare files of object
	{
		string objectName = objectDeclareValue[i]["objectName"].asString();
		string objectJson = objectDeclareValue[i]["objectJson"].asString();
		LogUtil::printInfo("Declare object " + objectName + " json:" + objectJson);
		declareObject.insert(std::make_pair(objectName, objectJson));
	}


	for (auto iter = declareObject.begin(); iter != declareObject.end(); iter++)
	{
		auto json = JsonLoader::getJsonFromFile(iter->second);

		GameObject* gameOriginObject = new GameObject(iter->first);

		gameOriginObject->shaderName = (*json)["shader"].asString();
		gameOriginObject->modelName = (*json)["model"].asString();

		gameObjectGroup.insert(std::make_pair(iter->first, gameOriginObject));
	}

	for (auto iter = gameObjectGroup.begin(); iter != gameObjectGroup.end(); iter++)
	{
		auto currentInstanceArray = objectInstanceValue[iter->first];//get array by origin object name
		for (int i = 0; i< currentInstanceArray.size(); i++)
		{
			auto curentInstance = currentInstanceArray[i];
			GameObject* newInstance = iter->second->getInstanceClone();

			//Load transform info from json
			string tagName = curentInstance["tagName"].asString();
			auto transformValue = curentInstance["transform"];
			auto positionValue = transformValue["position"];
			auto scaleValue = transformValue["scale"];
			auto rotationValue = transformValue["rotation"];


			newInstance->transform.position = glm::vec3(positionValue[0].asFloat(), positionValue[1].asFloat(), positionValue[2].asFloat());
			newInstance->transform.rotation = glm::vec3(rotationValue[0].asFloat(), rotationValue[1].asFloat(), rotationValue[2].asFloat());
			newInstance->transform.scale = glm::vec3(scaleValue[0].asFloat(), scaleValue[1].asFloat(), scaleValue[2].asFloat());
			newInstance->tagName = tagName;
			if (newInstance->modelName.length() > 0)
			{
				newInstance->objectModel = new Model("assets/Model/object/" + newInstance->modelName);
			}
			//newInstance->onRenderInit();
			gameInstanceGroup.insert(std::make_pair(tagName, newInstance));
		}
		
	}
}

void GameObjectManager::onLogicalWork()
{
	onLogicalInit();

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
	{
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

	while (!HorizonFrame::getInstance()->getFrameTerminate())
	{
		for (auto iter = gameInstanceGroup.begin(); iter != gameInstanceGroup.end(); iter++)
		{
			std::unique_lock<mutex> lock(instanceMutex);
			iter->second->onUpdate();
		}
	}
	onLogicalFinish();

	LuaUtil::luaEnvironmentRelease(luaState);
}

void GameObjectManager::onRenderWork()
{
	for (auto iter = gameInstanceGroup.begin(); iter != gameInstanceGroup.end(); iter++)
	{
		std::unique_lock<mutex> lock(instanceMutex);
		GameObject* object = iter->second;
		if (!object->haveRenderInit) {
			object->onRenderInit();
		}
		if (!object->isDead){
			iter->second->onRender();
		}
		
	}
}

void GameObjectManager::onLogicalFinish()
{

}

