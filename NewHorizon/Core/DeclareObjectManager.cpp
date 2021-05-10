#include "DeclareObjectManager.h"
#include "../Util/JsonLoader.h"
#include "../Util/LogUtil.hpp"
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
