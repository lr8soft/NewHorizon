
#include <iostream>
#include "GameObject.h"
#include "../Util/JsonLoader.h"
#include "../FrameInfo.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Util/LogUtil.hpp"

#include "GameObjectBinder.h"
#include "ModelManager.h"
#include "RenderManager.h"

GameObject* GameObject::getInstanceClone()
{
	GameObject* clone = new GameObject;
	clone->classObject = classObject;

	return clone;
}

void GameObject::onRenderInit()
{
	if (!haveRenderInit)
	{
		ModelManager::getInstance()->InitModel(classObject->modelName);

		haveRenderInit = true;
	}
}

void GameObject::onRender()
{
	glm::mat4 matrix;
	matrix = glm::translate(matrix, transform.position);
	matrix = glm::scale(matrix, transform.scale);
	if (transform.rotation.x != 0)
		matrix = glm::rotate(matrix, transform.rotation.x, glm::vec3(1, 0, 0));
	if (transform.rotation.y != 0)
		matrix = glm::rotate(matrix, transform.rotation.y, glm::vec3(0, 1, 0));
	if (transform.rotation.z != 0)
		matrix = glm::rotate(matrix, transform.rotation.z, glm::vec3(0, 0, 1));

	RenderManager* renderManager = RenderManager::getInstance();
	renderManager->setModelMatrix(&matrix);
	renderManager->onRender(classObject);

}

void GameObject::onUpdate(lua_State* luaState)
{
	objectTimer.Tick();

	GameObjectBinder::invokeFixedUpdate(luaState, classObject->scriptNameSpace.c_str());
}

void GameObject::onRenderRelease()
{

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



