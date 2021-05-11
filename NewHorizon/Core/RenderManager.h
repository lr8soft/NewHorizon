#pragma once
#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include <glm/glm.hpp>
#include <vector>

class GameObject;
class RenderManager {
private:
	glm::mat4* modelMatrix;

	std::vector<GameObject*> directionalLightGroup, pointLightGroup, flashLightGroup;

	static RenderManager* pInstance;

	void sendLightCount(unsigned int);

public:
	static RenderManager* getInstance();

	void setModelMatrix(glm::mat4*);
	void onRender(GameObject*);
	void onFinishRender();

};

#endif