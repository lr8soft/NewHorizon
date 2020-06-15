#include <iostream>
#include <GL3/gl3w.h>
#include "HorizonFrame.h"
#include "FrameInfo.h"

#include "Core/GameObject.h"
HorizonFrame* HorizonFrame::pInstance = nullptr;

HorizonFrame::HorizonFrame()
{
}
void HorizonFrame::FrameInit()
{
	float smallSize = FrameInfo::ScreenWidth > FrameInfo::ScreenHeight ? FrameInfo::ScreenHeight : FrameInfo::ScreenWidth;
	float absWidth = smallSize / (float)FrameInfo::ScreenWidth;
	float absHeight = smallSize / (float)FrameInfo::ScreenHeight;

	FrameInfo::FrameRight = absWidth;
	FrameInfo::FrameLeft = -absWidth;
	FrameInfo::FrameTop = absHeight;
	FrameInfo::FrameBottom = -absHeight;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, false);//Auto change size

	GLFWmonitor* primaryMonitor = isFullScreen ? glfwGetPrimaryMonitor() : nullptr;
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	tScreen = glfwCreateWindow(1, 1, "ThreadInitHelper", nullptr, nullptr);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	pScreen = glfwCreateWindow(FrameInfo::ScreenWidth, FrameInfo::ScreenHeight, "NewHorizon", primaryMonitor, tScreen);
	glfwMakeContextCurrent(pScreen);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowPos(pScreen, (mode->width - FrameInfo::ScreenWidth) / 2, (mode->height - FrameInfo::ScreenHeight) / 2);
	if (pScreen == nullptr) {
		std::cerr << "[ERROR] Failed to create window!\n";
		exit(-1);
	}

	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(pScreen, FrameResize);
	glfwSetWindowPosCallback(pScreen, FramePos);
	gl3wInit();
}

void HorizonFrame::FrameLoop()
{

	GameObject* object = new GameObject("test");
	object->onAssetsInit();
	object->onRenderInit();

	while (!glfwWindowShouldClose(pScreen)) {
		glClear(GL_COLOR_BUFFER_BIT);
		timer.Tick();

		object->onUpdate();
		object->onRender();

		glfwSwapBuffers(pScreen);
		glfwPollEvents();
	}

	glfwDestroyWindow(pScreen);
	glfwDestroyWindow(tScreen);
	glfwTerminate();
}

void HorizonFrame::FrameFinalize()
{
	delete pInstance;
}


void HorizonFrame::FrameResize(GLFWwindow * screen, int w, int h)
{
	if (w > 0 && h > 0) {
		glViewport(0, 0, w, h);

		float smallSize = w > h ? h : w;
		float absWidth = smallSize / (float)w;
		float absHeight = smallSize / (float)h;

		FrameInfo::FrameRight = absWidth;
		FrameInfo::FrameLeft = -absWidth;
		FrameInfo::FrameTop = absHeight;
		FrameInfo::FrameBottom = -absHeight;

		FrameInfo::ScreenHeight = h;
		FrameInfo::ScreenWidth = w;
	}
}

void HorizonFrame::FramePos(GLFWwindow * screen, int x, int y)
{
}

HorizonFrame * HorizonFrame::getInstance()
{
	if (pInstance == nullptr) {
		pInstance = new HorizonFrame;
	}
	return pInstance;
}

GLFWwindow * HorizonFrame::getScreen()
{
	return pScreen;
}
