#include "EngineManager.h"
#include "../Util/JsonLoader.h"
#include "../Util/LogUtil.hpp"
#include "../HorizonFrame.h"
#include <thread>

#include <ctime>

#include "../Util/MathUtil.h"
#include "../Util/LuaUtil.h"
#include "../ThirdParty/lua/lua.hpp"
#include "GameObjectBinder.h"
#include "DeclareObjectManager.h"
#include "ModelManager.h"
#include "RenderManager.h"
#include "AudioManager.h"

#include "EngineDefine.h"


using namespace std;
EngineManager* EngineManager::pInstance = nullptr;

EngineManager::EngineManager(){}
EngineManager * EngineManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new EngineManager;
	}
	return pInstance;
}

void EngineManager::onLogicalInit()
{
	auto configJson = JsonLoader::getJsonFromFile(INSTANCE_CONFIG_FILE);
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

	DeclareObjectManager* declareManager = DeclareObjectManager::getInstance();
	for (auto iter = declareObject.begin(); iter != declareObject.end(); iter++)	//load all declareobject from config json
	{
		string originObjectName = iter->first;

		GameObject* gameOriginObject = new GameObject;


		DeclareObject* classObject = declareManager->LoadDeclareObject(originObjectName, iter->second);
		gameOriginObject->classObject = classObject;



		string scriptName = gameOriginObject->classObject->scriptName;
		if (scriptName.length() > 0) {//load lua script
			int status = luaL_loadfile(luaState, (DECLARE_OBJECT_CONFIG_FOLDER + scriptName).c_str());
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

	ModelManager* modelManager = ModelManager::getInstance();
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

			if (newInstance->classObject->modelName.length() > 0)//add model object
			{
				modelManager->LoadModel(newInstance->classObject->modelName);
			}
			gameInstanceGroup.insert(std::make_pair(tagName, newInstance));
		}
		
	}
}


void EngineManager::onLogicalWork()
{
	luaState = LuaUtil::getNewGameObjectEvon();
	AudioManager* audioManager = AudioManager::getInstance();
	
	onLogicalInit();
	audioManager->LoadAllAudios();
	while (!HorizonFrame::getInstance()->getFrameTerminate())
	{
		timer.Tick();
		float currentTime = timer.getAccumlateTime();
		if (currentTime - lastUpdateTime >= 0.016666667)//60 tick per second
		{
			for (auto iter = gameInstanceGroup.begin(); iter != gameInstanceGroup.end(); iter++)
			{
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
					gameInstanceGroup.insert(std::make_pair(asyncIter->first, asyncIter->second));
				}
				gameInstanceGroup.clear();
			}
			gameCamera.processInput((currentTime - lastUpdateTime));//return real delta time
			lastUpdateTime = timer.getAccumlateTime();
		}
	}
	onLogicalFinish();

	LuaUtil::luaEnvironmentRelease(luaState);
}

void EngineManager::applyRenderSettings()
{
	RenderManager::getInstance()->applyRenderSettings();
}

void EngineManager::onRenderWork()
{
	RenderManager::getInstance()->onStartRender();
	for (auto iter = gameInstanceGroup.begin(); iter != gameInstanceGroup.end(); iter++)
	{
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

void EngineManager::onMouseUpdate(double x, double y)
{
	gameCamera.processMouse(x, y);
}

void EngineManager::onScrollUpdate(double x, double y)
{
	gameCamera.processScroll(y);
}

Camera * EngineManager::getCamera()
{
	return &gameCamera;
}

GameObject * EngineManager::addGameObjectInstance(const std::string & originObjectName, const std::string & tagName)
{
	auto originObjectIter = gameObjectGroup.find(originObjectName);
	if(originObjectIter != gameObjectGroup.end())
	{
		auto testInstanceIter = gameInstanceGroup.find(tagName);
		if (testInstanceIter == gameInstanceGroup.end())//if tagName existed
		{
			GameObject* newInstance = originObjectIter->second->getInstanceClone();

			newInstance->tagName = tagName;
			if (newInstance->classObject->modelName.length() > 0)//add model object
			{
				ModelManager::getInstance()->LoadModel(newInstance->classObject->modelName);
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

void EngineManager::onLogicalFinish()
{

}

