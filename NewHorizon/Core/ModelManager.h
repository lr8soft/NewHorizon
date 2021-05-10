#pragma once
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

#include <string>
#include <map>
#include "Model/Model.h"
class ModelManager {
private:
	std::map<std::string, Model*> globalModelGroup;

	static ModelManager* pInstance;


	Model* GetModel(const std::string& modelFileName);
public:
	static ModelManager* getInstance();

	void LoadModel(const std::string& modelFileName);
	void RenderModel(const std::string& modelFileName);
};

#endif