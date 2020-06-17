#pragma once
#ifndef _RENDER_MANAGER_
#define _RENDER_MANAGER_
class RenderManager {
private:
	static RenderManager* pInstance;

	RenderManager();
public:
	static RenderManager* getInstance();
};

#endif