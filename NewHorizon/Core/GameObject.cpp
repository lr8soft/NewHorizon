#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "../Util/JsonLoader.h"
GameObject::GameObject(const std::string & objectName) : objectName(objectName)
{
}

void GameObject::onInit()
{
	auto json = JsonLoader::getJsonFromFile("assets/Config/object/" + objectName + ".json");

	shaderName = (*json)["shader"].asString();

	Json::Value textureInfo = (*json)["texture"];
	std::string ambientTex = textureInfo["ambient"].asString();
	std::string diffuseTex = textureInfo["diffuse"].asString();
	std::string specularTex = textureInfo["specular"].asString();
	float shininess = textureInfo["shininess"].asFloat();

	std::cout << "shader:"<<shaderName << " ambient:" << ambientTex << " diffuse:" << diffuseTex << " specular:" << specularTex << " shininess:" << shininess << std::endl;

	/*xc_ogl::ShaderReader shader;
	shader.loadFromFile(("assets/Shader/object/" + objectName + ".vert").c_str(), GL_VERTEX_SHADER);
	shader.loadFromFile(("assets/Shader/object/" + objectName + ".frag").c_str(), GL_FRAGMENT_SHADER);
	shader.linkAllShader();

	shaderHandle = shader.getProgramHandle();*/
}

void GameObject::onRender()
{
}


void GameObject::onUpdate()
{
}

void GameObject::onRelease()
{
}



