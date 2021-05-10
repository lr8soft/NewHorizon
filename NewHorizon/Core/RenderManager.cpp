#include "RenderManager.h"

RenderManager* RenderManager::pInstance = nullptr;

RenderManager * RenderManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new RenderManager;
	}
	return pInstance;
}
