#include "LogicalManager.h"
#include "../Util/JsonLoader.h"
#include "../Util/LogUtil.hpp"
using namespace std;
LogicalManager* LogicalManager::pInstance = nullptr;

LogicalManager::LogicalManager(){}
LogicalManager * LogicalManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new LogicalManager;
	}
	return pInstance;
}

void LogicalManager::onLogicalInit()
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
			auto positionValue = curentInstance["position"];
			auto scaleValue = curentInstance["scale"];
			auto rotationValue = curentInstance["rotation"];

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

void LogicalManager::onLogicalWork()
{

	for (auto iter = gameInstanceGroup.begin(); iter != gameInstanceGroup.end(); iter++)
	{
		iter->second->onUpdate();
		iter->second->onRender();
	}
}

void LogicalManager::onLogicalFinish()
{
}

