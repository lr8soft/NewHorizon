#pragma once
#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include <glm/glm.hpp>
#include <list>

class Skybox;
class GameObject;
class RenderManager {
private:
	std::list<GameObject*> directionalLightGroup, pointLightGroup, flashLightGroup;

	unsigned int depthMapFBO, depthMap;

	glm::mat4* modelMatrix;

	Skybox* skyboxObject;

	static RenderManager* pInstance;

	static inline bool findObject(std::list<GameObject*>& group, GameObject* object)
	{
		auto iterEnd = group.end();
		return (std::find(group.begin(), iterEnd, object) == iterEnd);
	}

	void onInitShadow();

	void renderDepthGraph(GameObject* gameObject);

	void sendShadowInfo(unsigned int, GameObject*);
	void sendLightInfo(unsigned int);

public:
	static RenderManager* getInstance();

	void setModelMatrix(glm::mat4*);
	void onRender(GameObject*);

	void applyRenderSettings();
	void onStartRender();
	void onFinishRender();

};

#endif