#include "DeclareObjectManager.h"
#include "../Util/JsonLoader.h"
#include "../Util/LogUtil.hpp"

#include "EngineDefine.h"

using namespace std;
DeclareObjectManager* DeclareObjectManager::pInstance = nullptr;

DeclareObjectManager * DeclareObjectManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new DeclareObjectManager;
	}
	return pInstance;
}

DeclareObject * DeclareObjectManager::LoadDeclareObject(const std::string& name, const std::string & path)
{
	DeclareObject* sameDeclareObject = GetDeclareObject(name);
	if (sameDeclareObject)
	{
		return sameDeclareObject;
	}

	auto json = JsonLoader::getJsonFromFile(path);

	string shaderName = (*json)["shader"].asString();
	string modelName = (*json)["model"].asString();
	string typeName = (*json)["type"].asString();
	string scriptName = (*json)["script"].asString();


	string scriptNameSpace;
	if (scriptName.length() > 0) {//load lua script
		scriptNameSpace = name;
	}
	else {
		scriptName = "default.lua";
		scriptNameSpace = "Default";
	}

	DeclareObject* declareObject = new DeclareObject;
	declareObject->assetName = name;
	declareObject->modelName = modelName;
	declareObject->scriptName = scriptName;
	declareObject->scriptNameSpace = scriptNameSpace;
	declareObject->shaderName = shaderName;
	declareObject->typeName = typeName;

	auto lightFloatData = (*json)["lightData"];

	LogUtil::printInfo(scriptNameSpace + " is " + typeName);
	if (typeName == POINT_LIGHT_TYPE_NAME)
	{
		auto ambient = lightFloatData["ambient"];
		declareObject->lightVectorData["ambient"] = glm::vec3(ambient[0].asFloat(), ambient[1].asFloat(), ambient[2].asFloat());

		auto diffuse = lightFloatData["diffuse"];
		declareObject->lightVectorData["diffuse"] = glm::vec3(diffuse[0].asFloat(), diffuse[1].asFloat(), diffuse[2].asFloat());

		auto specular = lightFloatData["specular"];
		declareObject->lightVectorData["specular"] = glm::vec3(specular[0].asFloat(), specular[1].asFloat(), specular[2].asFloat());

		declareObject->lightFloatData["constant"] = lightFloatData["constant"].asFloat();
		declareObject->lightFloatData["linear"] = lightFloatData["linear"].asFloat();
		declareObject->lightFloatData["quadratic"] = lightFloatData["quadratic"].asFloat();
	}
	else if (typeName == FLASH_LIGHT_TYPE_NAME)
	{
		auto direction = lightFloatData["direction"];
		declareObject->lightVectorData["direction"] = glm::vec3(direction[0].asFloat(), direction[1].asFloat(), direction[2].asFloat());

		declareObject->lightFloatData["cutOff"] = lightFloatData["cutOff"].asFloat();
		declareObject->lightFloatData["outerCutOff"] = lightFloatData["outerCutOff"].asFloat();

		declareObject->lightFloatData["constant"] = lightFloatData["constant"].asFloat();
		declareObject->lightFloatData["linear"] = lightFloatData["linear"].asFloat();
		declareObject->lightFloatData["quadratic"] = lightFloatData["quadratic"].asFloat();

		auto ambient = lightFloatData["ambient"];
		declareObject->lightVectorData["ambient"] = glm::vec3(ambient[0].asFloat(), ambient[1].asFloat(), ambient[2].asFloat());

		auto diffuse = lightFloatData["diffuse"];
		declareObject->lightVectorData["diffuse"] = glm::vec3(diffuse[0].asFloat(), diffuse[1].asFloat(), diffuse[2].asFloat());

		auto specular = lightFloatData["specular"];
		declareObject->lightVectorData["specular"] = glm::vec3(specular[0].asFloat(), specular[1].asFloat(), specular[2].asFloat());
	}
	else if (typeName == DIRECTIONAL_LIGHT_TYPE_NAME)
	{
		auto direction = lightFloatData["direction"];
		declareObject->lightVectorData["direction"] = glm::vec3(direction[0].asFloat(), direction[1].asFloat(), direction[2].asFloat());

		auto ambient = lightFloatData["ambient"];
		declareObject->lightVectorData["ambient"] = glm::vec3(ambient[0].asFloat(), ambient[1].asFloat(), ambient[2].asFloat());

		auto diffuse = lightFloatData["diffuse"];
		declareObject->lightVectorData["diffuse"] = glm::vec3(diffuse[0].asFloat(), diffuse[1].asFloat(), diffuse[2].asFloat());

		auto specular = lightFloatData["specular"];
		declareObject->lightVectorData["specular"] = glm::vec3(specular[0].asFloat(), specular[1].asFloat(), specular[2].asFloat());
	}
	else if (typeName == SKYBOX_TYPE_NAME)
	{
		auto skyboxData = (*json)["skyboxData"];
		declareObject->declareStrData["top"] = TEXTURE_FOLDER + skyboxData["top"].asString();
		declareObject->declareStrData["bottom"] = TEXTURE_FOLDER + skyboxData["bottom"].asString();
		declareObject->declareStrData["left"] = TEXTURE_FOLDER + skyboxData["left"].asString();
		declareObject->declareStrData["right"] = TEXTURE_FOLDER + skyboxData["right"].asString();
		declareObject->declareStrData["front"] = TEXTURE_FOLDER + skyboxData["front"].asString();
		declareObject->declareStrData["back"] = TEXTURE_FOLDER + skyboxData["back"].asString();
	}
	else {
		declareObject->lightFloatData["shininess"] = lightFloatData["shininess"].asFloat();
	}

	declareObjectGroup[name] = declareObject;	//insert new declareObject

	return declareObject;
}

DeclareObject * DeclareObjectManager::GetDeclareObject(const std::string & name)
{
	if (declareObjectGroup.find(name) != declareObjectGroup.end())	//existed
	{
		return declareObjectGroup[name];
	}

	return nullptr;
}
