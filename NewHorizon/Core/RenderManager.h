#pragma once
#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_
#include <glm/glm.hpp>
#include "DeclareObject.h"

class RenderManager {
private:
	glm::mat4* modelMatrix;

	static RenderManager* pInstance;


public:
	static RenderManager* getInstance();

	void setModelMatrix(glm::mat4*);
	void onRender(DeclareObject*);

};

#endif