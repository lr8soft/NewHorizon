#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "ShaderHelper.h"
#include "../Util/JsonLoader.h"
#include "../FrameInfo.h"

#include <GL3/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Util/LogUtil.hpp"
GameObject::GameObject(const std::string & assetName) : assetName(assetName)
{
}

void GameObject::onAssetsInit()
{
	auto json = JsonLoader::getJsonFromFile("assets/Config/object/" + assetName + ".json");

	shaderName = (*json)["shader"].asString();
	modelName = (*json)["model"].asString();


	//Load transform info from json
	auto transformValue = (*json)["transform"];
	auto positionValue = transformValue["position"];
	auto scaleValue = transformValue["scale"];
	auto rotationValue = transformValue["rotation"];

	transform.position = glm::vec3(positionValue[0].asFloat(), positionValue[1].asFloat(), positionValue[2].asFloat());
	transform.rotation = glm::vec3(rotationValue[0].asFloat(), rotationValue[1].asFloat(), rotationValue[2].asFloat());
	transform.scale = glm::vec3(scaleValue[0].asFloat(), scaleValue[1].asFloat(), scaleValue[2].asFloat());

	if (modelName.length() > 0)
	{
		objectModel = new Model("assets/Model/object/" + modelName);
	}
}

void GameObject::onRenderInit()
{
	if (objectModel != nullptr)
	{
		objectModel->onModelInit();
	}
}

void GameObject::onRender()
{
	if (objectModel != nullptr)
	{
		GLuint shader = ShaderHelper::getInstance()->bindProgram("object", shaderName);

		glm::mat4 matrix;
		matrix = glm::translate(matrix, transform.position);
		matrix = glm::scale(matrix, transform.scale * glm::vec3(FrameInfo::FrameRight, FrameInfo::FrameTop, 1.0f));
		if(transform.rotation.x != 0)
			matrix = glm::rotate(matrix, transform.rotation.x, glm::vec3(1, 0, 0));
		if(transform.rotation.y != 0)
			matrix = glm::rotate(matrix, transform.rotation.y, glm::vec3(0, 1, 0));
		if (transform.rotation.z != 0)
			matrix = glm::rotate(matrix, transform.rotation.z, glm::vec3(0, 0, 1));
		glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewProjection"), 1, false, glm::value_ptr(matrix));

		objectModel->onModelRender(shader);
	}
}

void GameObject::onUpdate()
{
	objectTimer.Tick();
	transform.rotation.y = objectTimer.getAccumlateTime();
}

void GameObject::onRelease()
{
	if (objectModel != nullptr)
	{
		objectModel->onModelRelease();
	}
}



