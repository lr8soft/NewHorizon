#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "ShaderManager.h"
#include "../Util/JsonLoader.h"
#include "../FrameInfo.h"

#include <GL3/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
GameObject::GameObject(const std::string & assetName) : assetName(assetName)
{
}

void GameObject::onAssetsInit()
{
	auto json = JsonLoader::getJsonFromFile("assets/Config/object/" + assetName + ".json");

	shaderName = (*json)["shader"].asString();
	modelName = (*json)["model"].asString();

	objectModel = new Model("assets/Model/object/" + modelName);
}

void GameObject::onRenderInit()
{
	objectModel->onModelInit();
}

void GameObject::onRender()
{
	GLuint shader = ShaderManager::getInstance()->bindProgram("object", shaderName);

	glm::mat4 matrix;
	matrix = glm::translate(matrix, glm::vec3(0.0f));
	matrix = glm::scale(matrix, glm::vec3(0.01f) * glm::vec3(FrameInfo::FrameRight, FrameInfo::FrameTop, 1.0f));
	matrix = glm::rotate(matrix, (float)objectTimer.getAccumlateTime(), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewProjection"), 1, false ,glm::value_ptr(matrix));
	objectModel->onModelRender(shader);

}

void GameObject::onUpdate()
{
	objectTimer.Tick();
}

void GameObject::onRelease()
{
}



