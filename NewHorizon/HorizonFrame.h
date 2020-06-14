#pragma once
#ifndef _FRAME_WORK_
#define _FRAME_WORK_

#include <GLFW/glfw3.h>
#include "Util/Timer.h"
class HorizonFrame 
{
private:
	/*STATIC AREA*/
	static HorizonFrame *pInstance;
	static void FrameResize(GLFWwindow* screen, int w, int h);
	static void FramePos(GLFWwindow* screen, int x, int y);

	GLFWwindow *pScreen = nullptr, *tScreen = nullptr;
	bool isFullScreen = false;

	Timer timer;
	HorizonFrame();

public:
	static HorizonFrame* getInstance();

	GLFWwindow* getScreen();

	void FrameInit();
	void FrameLoop();
	void FrameFinalize();
};

#endif