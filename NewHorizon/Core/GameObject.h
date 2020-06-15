#pragma once
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_
#include <string>
#include <GL/glcorearb.h>
class GameObject {
protected:
	std::string objectName, shaderName;
	GLuint shaderHandle;
public:
	GameObject(const std::string& objectName);
	virtual void onInit();
	virtual void onUpdate();
	virtual void onRender();
	virtual void onRelease();
};

#endif