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


GameObject* GameObject::getInstanceClone()
{
	GameObject* clone = new GameObject(assetName);
	clone->modelName = modelName;
	clone->shaderName = shaderName;
	return clone;
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
	LogUtil::printInfo(std::to_string(objectModel != nullptr));
}

void GameObject::onUpdate()
{
	objectTimer.Tick();
	transform.rotation.y = objectTimer.getAccumlateTime();
	//transform.rotation.x = objectTimer.getAccumlateTime() * 0.5;
}

void GameObject::onRelease()
{
	if (objectModel != nullptr)
	{
		objectModel->onModelRelease();
	}
}



