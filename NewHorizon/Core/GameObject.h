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
struct Transform {
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 motion = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
	glm::vec3 rotation = glm::vec3(0.0);
};
class GameObject {
protected:
	std::map<std::string, IComponent*> componentMap;// component group
	std::string assetName, shaderName, modelName;
	GLuint shaderHandle;

	Timer objectTimer;
	Model* objectModel;

	Transform transform;
public:
	GameObject(const std::string& assetName);
	virtual void onAssetsInit();
	virtual void onRenderInit();

	virtual void onUpdate();
	virtual void onRender();
	virtual void onRelease();
};

#endif