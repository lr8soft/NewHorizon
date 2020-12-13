#pragma once
#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_
#include <map>
#include <vector>
#include <string>

#include "GameObject.h"
#include "Camera.h"
#include "../Util/Timer.h"
#include "../ThirdParty/lua/lua.hpp"
class GameObjectManager {
private:
	static GameObjectManager* pInstance;

	std::map<std::string, GameObject*> gameInstanceGroup;//tagName, instance
	std::map<std::string, GameObject*> gameObjectGroup;//objectName, originInstance

	std::map<std::string, GameObject*> asyncInstanceGroup;//tagName, instance, will add to gameInstanceGroup next tick

	Camera gameCamera;

	//std::mutex instanceMutex;
	
	void onLogicalInit();
	void onLogicalFinish();

	Timer timer;
	float lastUpdateTime = 0.0f;

	lua_State* luaState;
	
	GameObjectManager();
public:
	static GameObjectManager* getInstance();
	void onLogicalWork();
	void onRenderWork();

	void onMouseUpdate(double x, double y);
	void onScrollUpdate(double x, double y);

	Camera* getCamera();
	
	GameObject* addGameObjectInstance(const std::string& originObjectName, const std::string& tagName);
};
#endif