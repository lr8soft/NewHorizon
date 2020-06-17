#pragma once
#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include "GameObject.h"
class LogicalManager {
private:
	static LogicalManager* pInstance;

	std::mutex logic_mt; // 互斥量，在执行逻辑任务的时候上锁，在解锁之前，下一次的渲染任务必须阻塞并等待被唤醒
	std::condition_variable logic_cv; // 逻辑线程的条件变量，满足条件后被渲染线程唤醒
	std::condition_variable render_cv; // 渲染线程的条件变量，满足条件后被逻辑线程唤醒

	bool to_do_logic = false; // true表示满足执行逻辑任务的条件，false表示不满足
	bool to_do_render = false; // true表示满足执行渲染任务的条件，false表示不满足

	std::map<std::string, GameObject*> gameInstanceGroup;//tagName, instance
	std::map<std::string, GameObject*> gameObjectGroup;//objectName, originInstance

	void GetFilesFromDirectory(std::vector<std::string> &files, const char *directoryPath);

	LogicalManager();
public:
	static LogicalManager* getInstance();

	void onLogicalInit();
	void onLogicalWork();
	void onLogicalFinish();
};
#endif