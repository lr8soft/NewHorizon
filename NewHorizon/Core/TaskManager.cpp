#include "TaskManager.h"
TaskManager* TaskManager::pInstance = nullptr;

TaskManager::TaskManager(){}
TaskManager * TaskManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new TaskManager;
	}
	return pInstance;
}

