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
#include "GameObjectManager.h"
GameObject::GameObject(const std::string & assetName) : assetName(assetName)
{
}


GameObject* GameObject::getInstanceClone()
{
	GameObject* clone = new GameObject(assetName);
	clone->modelName = modelName;
	clone->shaderName = shaderName;
	clone->scriptName = scriptName;
	clone->scriptNameSpace = scriptNameSpace;

	return clone;
}

void GameObject::onRenderInit()
{
	if (!haveRenderInit && objectModel != nullptr)
	{
		std::unique_lock<std::mutex> lock(gameObjectMutex);

		objectModel->onModelInit();
		haveRenderInit = true;
	}
}

glm::vec3 GameObject::lightPos = glm::vec3(1.2f, 1.0f, 1.0f);
void GameObject::onRender()
{
	if (objectModel != nullptr)
	{
		//std::unique_lock<std::mutex> lock(gameObjectMutex);

		GLuint shader = ShaderHelper::getInstance()->bindProgram("object", shaderName);

		glm::mat4 matrix;
		matrix = glm::translate(matrix, transform.position);
		matrix = glm::scale(matrix, transform.scale);
		if(transform.rotation.x != 0)
			matrix = glm::rotate(matrix, transform.rotation.x, glm::vec3(1, 0, 0));
		if(transform.rotation.y != 0)
			matrix = glm::rotate(matrix, transform.rotation.y, glm::vec3(0, 1, 0));
		if (transform.rotation.z != 0)
			matrix = glm::rotate(matrix, transform.rotation.z, glm::vec3(0, 0, 1));

		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(matrix));
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(GameObjectManager::getInstance()->getCamera()->getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(GameObjectManager::getInstance()->getCamera()->getProjectionMatrix()));


		glm::vec3 viewPos = GameObjectManager::getInstance()->getCamera()->getPostion();
		//light info
		//glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
		//glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
		//glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
		glUniform1f(glGetUniformLocation(shader, "material.shininess"), 32);

		glUniform3fv(glGetUniformLocation(shader, "light.direction"), 1, glm::value_ptr(glm::vec3(-3, -3, -3)));
		//glUniform3fv(glGetUniformLocation(shader, "light.position"), 1, glm::value_ptr(lightPos));
		glUniform3fv(glGetUniformLocation(shader, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
		glUniform3fv(glGetUniformLocation(shader, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
		glUniform3fv(glGetUniformLocation(shader, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		//glUniform3fv(glGetUniformLocation(shader, "ambientColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		//glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(glm::vec3(3, 3, 3)));//a lightpot in (2, 2, 2)
		//glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));//

		glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));

		objectModel->onModelRender(shader);
	}
	
}

void GameObject::onUpdate(lua_State* luaState)
{
	//std::unique_lock<std::mutex> lock(gameObjectMutex);

	objectTimer.Tick();

	lua_getglobal(luaState,	scriptNameSpace.c_str());//get the table of lua (assestName={})
	lua_pushstring(luaState, "onFixedUpdate");//push string "onFixedUpdate" to the vstack
	lua_gettable(luaState, -2);//get the object "onFixedUpdate" to the top of vstack
	lua_call(luaState, 0, 0);//call the function, 0 parameter 0 return

}

void GameObject::onRenderRelease()
{
	if (objectModel != nullptr)
	{
		std::unique_lock<std::mutex> lock(gameObjectMutex);
		objectModel->onModelRelease();
		objectModel = nullptr;
	}
}

bool GameObject::getIsRenderInit()
{
	return haveRenderInit;
}


void GameObject::setDead(bool isDead)
{
	this->isDead = isDead;
}

void GameObject::setTransform(Transform transform)
{
	lastTransform = this->transform;
	this->transform = transform;
	
}

Transform GameObject::getTransform()
{
	return transform;
}

std::string GameObject::getTagName()
{
	return tagName;
}

float GameObject::getDeltaTime()
{
	return objectTimer.getDeltaFrame();
}

long double GameObject::getAccmulateTime()
{
	return objectTimer.getAccumlateTime();
}



