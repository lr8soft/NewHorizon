#include "GameObjectManager.h"
#include "../Util/JsonLoader.h"
#include "../Util/LogUtil.hpp"
#include "../HorizonFrame.h"
#include <thread>

#include <ctime>
#include "../Util/LuaUtil.h"
#include "../ThirdParty/lua/lua.hpp"
#include "GameObjectBinder.h"
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

		string originObjectName = iter->first;

		GameObject* gameOriginObject = new GameObject(originObjectName);

		gameOriginObject->shaderName = (*json)["shader"].asString();
		gameOriginObject->modelName = (*json)["model"].asString();

		string scriptName = (*json)["script"].asString();
		gameOriginObject->scriptName = scriptName;

		if (scriptName.length() > 0) {//load lua script
			int status = luaL_loadfile(luaState, ("assets/Script/object/" + scriptName).c_str());
			if (status == LUA_OK)
			{
				lua_pcall(luaState, 0, LUA_MULTRET, 0);//call default lua function
				LogUtil::printInfo("Load script " + scriptName);
			}
			else
			{
				LogUtil::printError("Fail to load script " + scriptName);
			}
		}

		gameObjectGroup.insert(std::make_pair(originObjectName, gameOriginObject));
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
			if (newInstance->modelName.length() > 0)//add model object
			{
				newInstance->objectModel = new Model("assets/Model/object/" + newInstance->modelName);
			}
			gameInstanceGroup.insert(std::make_pair(tagName, newInstance));
		}
		
	}
}


void GameObjectManager::onLogicalWork()
{
	luaState = LuaUtil::getNewGameObjectEvon();
	onLogicalInit();
	while (!HorizonFrame::getInstance()->getFrameTerminate())
	{
		timer.Tick();
		float currentTime = timer.getAccumlateTime();
		if (currentTime - lastUpdateTime >= 0.016666667)//60 tick per second
		{
			for (auto iter = gameInstanceGroup.begin(); iter != gameInstanceGroup.end(); iter++)
			{
				std::unique_lock<mutex> lock(instanceMutex);
				GameObject* currentGameObject = iter->second;
				if (!currentGameObject->isDead)
				{
					GameObjectBinder::setCurrentInstance(iter->second);
					iter->second->onUpdate(luaState);
				}

			}

			if (!asyncInstanceGroup.empty())//add to logical group
			{
				for (auto asyncIter = asyncInstanceGroup.begin(); asyncIter != asyncInstanceGroup.end(); asyncIter++)
				{
					std::unique_lock<mutex> lock(instanceMutex);
					gameInstanceGroup.insert(std::make_pair(asyncIter->first, asyncIter->second));
				}
				gameInstanceGroup.clear();
			}
			gameCamera.processInput(timer.getDeltaFrame());
			lastUpdateTime = timer.getAccumlateTime();
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

		if (object->isDead)
		{
			object->onRenderRelease();
			if (std::next(iter) == gameInstanceGroup.end())
			{
				gameInstanceGroup.erase(iter);
				break;
			}
			else {
				gameInstanceGroup.erase(iter++);
			}
			delete object;
		}
		
	}
}

Camera * GameObjectManager::getCamera()
{
	return &gameCamera;
}

GameObject * GameObjectManager::addGameObjectInstance(const std::string & originObjectName, const std::string & tagName)
{
	auto originObjectIter = gameObjectGroup.find(originObjectName);
	if(originObjectIter != gameObjectGroup.end())
	{
		auto testInstanceIter = gameInstanceGroup.find(tagName);
		if (testInstanceIter == gameInstanceGroup.end())//if tagName existed
		{
			GameObject* newInstance = originObjectIter->second->getInstanceClone();

			newInstance->tagName = tagName;
			if (newInstance->modelName.length() > 0)//add model object
			{
				newInstance->objectModel = new Model("assets/Model/object/" + newInstance->modelName);
			}
			gameInstanceGroup.insert(std::make_pair(tagName, newInstance));

			return newInstance;
		}
		else {
			return testInstanceIter->second;
		}
	}
	LogUtil::printError("Unexisted origin object " + originObjectName);
	return nullptr;
}

void GameObjectManager::onLogicalFinish()
{

}

