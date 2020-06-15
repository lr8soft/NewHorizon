#pragma once
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_
#include <string>
#include <GL/glcorearb.h>
#include "../Util/Timer.h"
#include "Model/Model.h"
class GameObject {
protected:
	std::string assetName, shaderName, modelName;
	GLuint shaderHandle;

	Model* objectModel;
	Timer objectTimer;
public:
	GameObject(const std::string& assetName);
	virtual void onAssetsInit();
	virtual void onRenderInit();

	virtual void onUpdate();
	virtual void onRender();
	virtual void onRelease();
};

#endif