#pragma once
#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_
#include <map>
#include <string>
class TaskManager {
private:
	static TaskManager* pInstance;

	TaskManager();
public:
	static TaskManager* getInstance();


};
#endif