#pragma once
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_
#include <string>
#include <GL/glcorearb.h>
#include "../Util/Timer.h"
#include "Component/IComponent.h"
#include "Model/Model.h"
#include <mutex>
#include <glm/glm.hpp>

#include "DeclareObject.h"
#include "../ThirdParty/lua/lua.hpp"
struct Transform {
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 motion = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
	glm::vec3 rotation = glm::vec3(0.0);
};
class GameObjectManager;
class GameObject {
protected:
	static glm::vec3 lightPos; //test

	Transform transform, lastTransform;

	std::mutex gameObjectMutex;

	Timer objectTimer;//Tick by logical thread

	std::string tagName;//unique name

	DeclareObject *classObject;
	Model* objectModel;

	GLuint shaderHandle;

	bool haveRenderInit = false, isDead = false;

public:
	friend class GameObjectManager;

	virtual GameObject* getInstanceClone();
	virtual void onRenderInit();
	//TODO call lua function and update GameObject information
	virtual void onUpdate(lua_State* luaState);
	
	virtual void onRender();
	virtual void onRenderRelease();

	bool getIsRenderInit();

	void setDead(bool isDead);
	void setTransform(Transform transform);
	Transform getTransform();
	std::string getTagName();
	float getDeltaTime();
	long double getAccmulateTime();
};

#endif