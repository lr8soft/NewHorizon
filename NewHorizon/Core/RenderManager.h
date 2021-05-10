#pragma once
#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

class RenderManager {
private:
	static RenderManager* pInstance;


public:
	static RenderManager* getInstance();

};

#endif