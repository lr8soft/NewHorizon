#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "ShaderManager.h"
#include "../Util/JsonLoader.h"
GameObject::GameObject(const std::string & assetName) : assetName(assetName)
{
}

void GameObject::onAssetsInit()
{
	auto json = JsonLoader::getJsonFromFile("assets/Config/object/" + assetName + ".json");

	shaderName = (*json)["shader"].asString();

	Json::Value textureInfo = (*json)["texture"];
	std::string ambientTex = textureInfo["ambient"].asString();
	std::string diffuseTex = textureInfo["diffuse"].asString();
	std::string specularTex = textureInfo["specular"].asString();
	float shininess = textureInfo["shininess"].asFloat();
}

void GameObject::onRenderInit()
{

}

void GameObject::onRender()
{
	ShaderManager::getInstance()->bindProgram("object", shaderName);



}

void GameObject::onUpdate()
{

}

void GameObject::onRelease()
{
}



