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

	std::mutex logic_mt; // ����������ִ���߼������ʱ���������ڽ���֮ǰ����һ�ε���Ⱦ��������������ȴ�������
	std::condition_variable logic_cv; // �߼��̵߳�����������������������Ⱦ�̻߳���
	std::condition_variable render_cv; // ��Ⱦ�̵߳����������������������߼��̻߳���

	bool to_do_logic = false; // true��ʾ����ִ���߼������������false��ʾ������
	bool to_do_render = false; // true��ʾ����ִ����Ⱦ�����������false��ʾ������

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