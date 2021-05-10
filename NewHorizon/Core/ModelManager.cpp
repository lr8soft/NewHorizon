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
	if (model == nullptr)	//load and init
	{
		model = new Model(GAME_OBJECT_MODEL_FOLDER + modelFileName);
		model->onModelInit();
		globalModelGroup[modelFileName] = model;
	}
}

void ModelManager::RenderModel(const std::string & modelFileName)
{
	Model* model = GetModel(modelFileName);
	if (model)
	{

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
