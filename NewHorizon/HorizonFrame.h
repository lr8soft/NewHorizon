#pragma once
#ifndef _FRAME_WORK_
#define _FRAME_WORK_

#include <GLFW/glfw3.h>
#include "Util/Timer.h"
#include <atomic>
class HorizonFrame 
{
private:
	/*STATIC AREA*/
	static HorizonFrame *pInstance;
	static void FrameResize(GLFWwindow* screen, int w, int h);
	static void FramePos(GLFWwindow* screen, int x, int y);
	static void FrameCurseUpdate(GLFWwindow* screen, double x, double y);
	static void FrameScrollUpdate(GLFWwindow* screen, double x, double y);

	GLFWwindow *pScreen = nullptr, *tScreen = nullptr;
	std::atomic_bool isFrameTerminate = false;
	bool isFullScreen = false;

	HorizonFrame();

public:
	static HorizonFrame* getInstance();
	bool getFrameTerminate();
	GLFWwindow* getScreen();

	void FrameInit();
	void FrameLoop();
	void FrameFinalize();
};

#endif