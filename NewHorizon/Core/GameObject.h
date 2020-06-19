#pragma once
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_
#include <string>
#include <GL/glcorearb.h>
#include "../Util/Timer.h"
#include "Component/IComponent.h"
#include "Model/Model.h"
#include <map>
#include <glm/glm.hpp>
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
	std::map<std::string, IComponent*> componentMap;// component group
	std::string assetName, shaderName, modelName, scriptName;
	std::string tagName;//unique name
	GLuint shaderHandle;

	Timer objectTimer;//Tick by logical thread
	Model* objectModel;
	bool haveRenderInit = false, isDead = false;
	Transform transform, lastTransform;
public:
	friend class GameObjectManager;
	
	GameObject(const std::string& assetName);
	virtual GameObject* getInstanceClone();
	virtual void onRenderInit();
	//TODO call lua function and update GameObject information
	virtual void onUpdate(lua_State* luaState);
	
	virtual void onRender();
	virtual void onRelease();

	bool getIsRenderInit();

	void setTransform(Transform transform);
	Transform getTransform();
	float getDeltaTime();
	long double getAccmulateTime();
};

#endif