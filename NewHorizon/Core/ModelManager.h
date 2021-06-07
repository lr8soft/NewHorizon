#pragma once
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

#include <string>
#include <map>
#include "Model/Model.h"
class ModelManager {
private:
	std::map<std::string, Model*> globalModelGroup;

	Model* GetModel(const std::string& modelFileName);

	static ModelManager* pInstance;
public:
	static ModelManager* getInstance();

	void LoadModel(const std::string& modelFileName);
	void InitModel(const std::string& modelFileName);
	void RenderModel(const std::string& modelFileName, unsigned int shader, bool useInsideTexture = true, unsigned int textureStartIndex = 0);

};

#endif