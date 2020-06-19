#pragma once
#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include "GameObject.h"
#include "../ThirdParty/lua/lua.hpp"
class GameObjectManager {
private:
	static GameObjectManager* pInstance;

	std::map<std::string, GameObject*> gameInstanceGroup;//tagName, instance
	std::map<std::string, GameObject*> gameObjectGroup;//objectName, originInstance

	std::mutex instanceMutex;
	
	void onLogicalInit();
	void onLogicalFinish();

	lua_State* luaState;
	
	GameObjectManager();
public:
	static GameObjectManager* getInstance();
	void onLogicalWork();
	void onRenderWork();
	
};
#endif