#include "ModelManager.h"
#include "EngineDefine.h"
ModelManager* ModelManager::pInstance = nullptr;
ModelManager * ModelManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new ModelManager;
	}
	return pInstance;
}

void ModelManager::LoadModel(const std::string & modelFileName)
{
	Model* model = GetModel(modelFileName);
	if (model == nullptr)
	{
		model = new Model(GAME_OBJECT_MODEL_FOLDER + modelFileName);
		globalModelGroup[modelFileName] = model;
	}
}

void ModelManager::InitModel(const std::string & modelFileName)
{
	Model* model = GetModel(modelFileName);
	if (model && !model->isModelInit())
	{
		model->onModelInit();
	}
}

void ModelManager::RenderModel(const std::string & modelFileName, unsigned int shader)
{
	Model* model = GetModel(modelFileName);
	if (model && model->isModelInit())
	{
		model->onModelRender(shader);
	}
}


Model * ModelManager::GetModel(const std::string & modelFileName)
{
	if (globalModelGroup.find(modelFileName) != globalModelGroup.end())
	{
		return globalModelGroup[modelFileName];
	}
	return nullptr;
}
